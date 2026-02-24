"""
Pytest configuration for the design patterns test suite.
"""
import sys
from pathlib import Path

# Find the project root by locating pyproject.toml
def find_project_root():
    """Find the project root by looking for pyproject.toml"""
    current = Path.cwd()

    # Search upward for pyproject.toml
    for parent in [current, *current.parents]:
        if (parent / "pyproject.toml").exists():
            return parent

    # Fallback: assume we're in the tests directory
    tests_dir = Path(__file__).parent
    potential_root = tests_dir.parent.parent  # python/ -> DesignPatterns/
    if (potential_root / "pyproject.toml").exists():
        return potential_root

    # Another fallback: from tests/ to DesignPatterns/
    return tests_dir.parent.parent


project_root = find_project_root()
src_path = project_root / "python" / "src"

# Add both the src path and its parent (so we can import from "src")
sys.path.insert(0, str(src_path))
sys.path.insert(0, str(src_path.parent))
