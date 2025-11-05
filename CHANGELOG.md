# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- **Aggregate mode** (`--cat` flag): Concatenate all files and pass to single Claude instance
- **Stdin mode** (`-` as first arg): Accept piped input for true aggregation
- Support for shell-expanded file arguments (e.g., `./c *.md 'task'`)
- Clear documentation on parallel vs aggregate modes
- Examples of two-stage parallel→aggregate workflows

### Changed
- Updated CLAUDE.md with accurate aggregation patterns
- Clarified when each Claude instance sees one file vs all files
- Added mode indicators in verbose output

### Fixed
- File argument parsing now handles both quoted globs and shell expansion
- Corrected misleading examples that suggested parallel mode could aggregate

## [1.0.0] - 2025-11-04

### Added
- Initial public release
- Core `c` script with parallel processing
- Comprehensive CLAUDE.md for Claude Code integration
- MIT License
- Contributing guidelines
- Full documentation

### Features
- Parallel processing (auto-detect CPU cores, max 16)
- Batch mode (-b flag for context clearing)
- Output directory mode (-o flag)
- Verbose mode (-v flag)
- Agentic mode (-a flag for auto-accept edits)
- Instruction from file (-i flag)
- GNU parallel integration (with xargs fallback)
- Comprehensive error handling
- Temp file management with cleanup
- Cross-platform support (macOS, Linux, WSL2)

---

## Version History

### Versioning Strategy

- **Major** (x.0.0): Breaking changes, major features
- **Minor** (0.x.0): New features, backwards compatible
- **Patch** (0.0.x): Bug fixes, documentation updates

### Release Notes Format

Each release includes:
- **Added**: New features
- **Changed**: Changes to existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Security fixes
