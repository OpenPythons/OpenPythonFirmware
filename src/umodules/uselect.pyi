# uselect
"wait for events on a set of streams"


class Poll:
    def register(self, obj, eventmask: int = 0):
        """Register obj for polling. eventmask is logical OR of:

        select.POLLIN - data available for reading
        select.POLLOUT - more data can be written
        select.POLLERR - error occurred
        select.POLLHUP - end of stream/connection termination detected
        eventmask defaults to select.POLLIN | select.POLLOUT.
        """

    def unregister(self, obj):
        "Unregister obj from polling."
        pass

    def modify(self, obj, eventmask: int):
        "Modify the eventmask for obj."
        pass

    def poll(timeout: int = None):
        """Wait for at least one of the registered objects to become ready. Returns list of (obj, event, …) tuples, event element specifies which events happened with a stream and is a combination of select.POLL* constants described above. There may be other elements in tuple, depending on a platform and version, so don’t assume that its size is 2. In case of timeout, an empty list is returned.
        Timeout is in milliseconds.
        """
        pass

    def ipoll(timeout: int = -1, flags: int = 0):
        """Like poll.poll(), but instead returns an iterator which yields callee-owned tuples. This function provides efficient, allocation-free way to poll on streams.
        If flags is 1, one-shot behavior for events is employed: streams for which events happened, event mask will be automatically reset (equivalent to poll.modify(obj, 0)), so new events for such a stream won’t be processed until new mask is set with poll.modify(). This behavior is useful for asynchronous I/O schedulers.
        """
        pass


def poll() -> Poll:
    "Create an instance of the Poll class."
    pass


def select(rlist: list, wlist: list, xlist: list, timeout: int = None):
    """Wait for activity on a set of objects.
    This function is provided by some MicroPython ports for compatibility and is not efficient. Usage of Poll is recommended instead.
    """
    pass
