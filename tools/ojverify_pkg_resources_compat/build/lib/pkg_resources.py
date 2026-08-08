"""Compatibility subset required by online-judge-verify-helper 5.6.0."""

from importlib.resources import files


def resource_string(package: str, resource: str) -> bytes:
    return files(package).joinpath(resource).read_bytes()
