"""Resolve tool imports and repository fixtures from the collected tests."""

from pathlib import Path
import sys


TOOLS_DIR = Path(__file__).resolve().parents[1]
REPOSITORY_ROOT = TOOLS_DIR.parent
sys.path.insert(0, str(TOOLS_DIR))
