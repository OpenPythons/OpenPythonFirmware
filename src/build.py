import io
import shutil
import sys
import traceback
from pathlib import Path
from subprocess import check_call, DEVNULL, CalledProcessError
from typing import TypeVar, Generic, Container

from dataclasses import dataclass
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection, Symbol

import re

FOLDER = Path(__file__).parent
BASE_FOLDER = FOLDER.parent
OPMOD_PATH = BASE_FOLDER / "opmod"
SOURCE_SYSCALL_TABLE: Path = OPMOD_PATH / "src/main/java/kr/pe/ecmaxp/openpython/arch/consts/OpenPythonSystemCallTable.kt"
TARGET_SYSCALL_TABLE: Path = FOLDER / "syscall_table.h"
TARGET_FOLDER: Path = OPMOD_PATH / "src/main/resources/assets/openpython/firmwares/v1.0.0"  # TODO: place version


@dataclass
class SymbolInfo:
    bind: str
    type: str


@dataclass
class SimpleSymbol:
    name: str
    st_name: int
    st_value: int
    st_size: int
    st_info: SymbolInfo
    st_other: Container
    st_shndx: int

    @classmethod
    def from_symbol(cls, symbol_obj: Symbol):
        return SimpleSymbol(
            symbol_obj.name,
            st_info=SymbolInfo(**symbol_obj.entry.pop("st_info")),
            **symbol_obj.entry,
        )


@dataclass
class SimpleSection:
    name: str
    sh_name: int
    sh_type: str
    sh_flags: int
    sh_addr: int
    sh_offset: int
    sh_size: int
    sh_link: int
    sh_info: int
    sh_addralign: int
    sh_entsize: int


@dataclass
class SimpleSegment:
    p_type: str
    p_offset: int
    p_vaddr: int
    p_paddr: int
    p_filesz: int
    p_memsz: int
    p_flags: int
    p_align: int


T = TypeVar("T")


def find_section(elf: ELFFile, cls: Generic[T]) -> T:
    return list(s for s in elf.iter_sections() if type(s) == cls)


def pretty_addr(addr: int, size=8) -> str:
    return "0x" + hex(addr)[2:].zfill(size)


def process_elf(elf: ELFFile, map_file: Path):
    if elf.get_machine_arch() != "ARM":
        raise Exception("Invalid machine arch {} (not ARM)".format(elf.get_machine_arch()))

    with map_file.open('w') as fp:
        type2typ = {
            "STT_OBJECT": "OBJECT",
            "STT_FUNC": "FUNC",
        }
        for symbol_table in find_section(elf, SymbolTableSection):
            symbols = [SimpleSymbol.from_symbol(obj) for obj in symbol_table.iter_symbols()
                       if (obj['st_info']['type'] == "STT_FUNC" or
                           obj['st_info']['type'] == "STT_OBJECT") and
                       obj['st_info']['bind'] == "STB_GLOBAL"]

            for symbol in sorted(symbols, key=lambda x: x.st_value):
                print(
                    pretty_addr(symbol.st_value),
                    pretty_addr(symbol.st_size, size=4),
                    # symbol.st_info.bind,
                    type2typ[symbol.st_info.type],
                    symbol.name,
                    sep="\t",
                    file=fp,
                )


def build(folder: Path = FOLDER, target_folder: Path = TARGET_FOLDER):
    build_path: Path = folder / "build"

    check_call(
        ["wsl", "make", "-j8"],
        cwd=str(folder),
        shell=True,
        stdin=DEVNULL
    )

    target_folder.mkdir(parents=True, exist_ok=True)

    with (build_path / "firmware.elf").open('rb') as fp:
        elf = ELFFile(fp)
        process_elf(elf, target_folder / "firmware.map")

    shutil.copyfile(str(build_path / "firmware.bin"), str(target_folder / "firmware.bin"))
    shutil.copyfile(str(build_path / "firmware.elf"), str(target_folder / "firmware.elf"))
    shutil.copyfile(str(build_path / "firmware.elf.map"), str(target_folder / "firmware.elf.map"))

    rom: Path = folder / "eeprom.py"
    text = rom.read_text()
    if text.startswith("# --"):
        text = text[len("# "):]
    else:
        raise Exception("eeprom missing magic tag")

    target_rom = (target_folder / "eeprom.py")
    target_rom.write_text(text)

    print(target_folder / "firmware.bin")


def parse_syscall_table(path: Path, file=sys.stdout):
    print(f"// syscall_table.h : {path.relative_to(BASE_FOLDER).as_posix()}", file=file)
    for line in SOURCE_SYSCALL_TABLE.read_text().splitlines():
        if line.startswith("package "):
            continue

        line, sep, comment = line.partition("//")
        line = line.strip()
        if not line:
            print(file=file)
            continue

        m = re.match("^const val (.*?) = (.*)$", line)
        if not m:
            continue

        print(f"#define {m.group(1)} ({m.group(2).replace('or', '|')})", file=file)


def main():
    fp = io.StringIO()
    parse_syscall_table(SOURCE_SYSCALL_TABLE, fp)
    if not TARGET_SYSCALL_TABLE.exists() or TARGET_SYSCALL_TABLE.read_text() != fp.getvalue():
        TARGET_SYSCALL_TABLE.write_text(fp.getvalue())

    try:
        build()
        return 0
    except CalledProcessError as e:
        traceback.print_exc()
        raise SystemExit(e.returncode)


if __name__ == '__main__':
    main()
