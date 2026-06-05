from datetime import datetime
import logging
from logging import StreamHandler, FileHandler, Formatter
from logging import INFO, DEBUG, NOTSET
from pathlib import Path

def get_logger(stream_handler=None, file_handler=None, force=True, level=INFO):
    if stream_handler is None:
        stream_handler = get_stream_handler(level)
    handlers = [stream_handler]
    if file_handler is not None:
        handlers.append(file_handler)

    logging.basicConfig(level=NOTSET, handlers=handlers, force=force)
    logger = logging.getLogger(__name__)
    return logger

def add_handler(logger, handler):
    if not isinstance(logger, logging.Logger):
        return
    logger.addHandler(handler)

def get_stream_handler(level):
    # Configures stream handler
    stream_handler = StreamHandler()
    stream_handler.setLevel(level)
    stream_handler.setFormatter(Formatter("%(message)s"))
    return stream_handler

def get_file_handler(filepath, filename=None, mode='a', encoding='utf-8', format=None, level=DEBUG):
    if not issubclass(type(filepath), Path):
        return
    filepath.mkdir(parents=True, exist_ok=True)

    # Configures file handler
    if filename is None:
        filename = f'log{datetime.now():%Y%m%d%H%M%S}.log'
    file_handler = FileHandler(filepath / filename, mode, encoding)
    file_handler.setLevel(level)
    if format is None:
        fmt = Formatter('%(message)s')
    else:
        fmt = Formatter(format)
    file_handler.setFormatter(fmt)
    return file_handler

def close_file_handler(logger, file_handler):
    file_handler.close()
    logger.removeHandler(file_handler)
