# Contributing to c-claude

Thank you for considering contributing to c-claude! This document provides guidelines for contributing.

---

## How to Contribute

### 1. Report Issues

Found a bug? Have a feature request?

- Check [existing issues](https://github.com/YOUR_USERNAME/c-claude/issues) first
- Open a new issue with clear description
- Include reproduction steps for bugs
- Include use case for feature requests

### 2. Submit Pull Requests

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/your-feature`
3. **Make your changes**
4. **Test thoroughly** (see Testing section)
5. **Commit**: Use clear commit messages
6. **Push**: `git push origin feature/your-feature`
7. **Open a Pull Request**

---

## Development Guidelines

### Code Style

**Shell Script**:
- Use `#!/bin/sh` for POSIX compatibility
- Indent with 2 spaces (not tabs)
- Quote variables: `"$VAR"` not `$VAR`
- Use `[ ]` for conditionals, not `[[ ]]` (POSIX)
- Check with `shellcheck` if available

**Example**:
```bash
if [ "$VAR" = "value" ]; then
  echo "Good"
fi
```

### Testing

Before submitting:

```bash
# 1. Test help command
./c help

# 2. Test basic execution
./c 'README.md' 'count words'

# 3. Test with flags
./c 'CLAUDE.md' 'summarize' -v
./c '*.md' 'count lines' -o /tmp/test/

# 4. Test error handling
./c  # Should show usage
./c 'nonexistent' 'test'  # Should error
./c 'c' -i /nonexistent  # Should error

# 5. Run shellcheck (if available)
shellcheck c
```

### Documentation

- Update README.md for user-facing changes
- Update CLAUDE.md for Claude Code integration changes
- Add comments for complex logic
- Update CHANGELOG.md

---

## Areas for Contribution

### High Priority

- [ ] **Windows native support** (PowerShell version)
- [ ] **Template library** (instruction templates for common tasks)
- [ ] **CI/CD integration examples** (GitHub Actions, GitLab CI)
- [ ] **Performance benchmarks** (more platforms, larger codebases)

### Medium Priority

- [ ] **Result aggregation utilities** (jq recipes, reporting tools)
- [ ] **Interactive mode** (select files from list)
- [ ] **Progress indicators** (show completion %)
- [ ] **Resume failed batches** (retry logic)

### Low Priority

- [ ] **Pre-commit hook integration**
- [ ] **VS Code extension** (right-click → run c-claude)
- [ ] **Web dashboard** (visualize results)
- [ ] **Configuration file** (.c-claude.yml for defaults)

---

## Commit Message Guidelines

Use conventional commits format:

```
type(scope): description

[optional body]

[optional footer]
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only
- `style`: Code style (formatting, no logic change)
- `refactor`: Code refactor (no feature change)
- `perf`: Performance improvement
- `test`: Add/update tests
- `chore`: Build process, dependencies, etc.

**Examples**:
```
feat: add progress indicator for long operations
fix: handle spaces in file paths correctly
docs: add Windows installation instructions
```

---

## Pull Request Process

1. **Update documentation** if adding features
2. **Add tests** for new functionality
3. **Update CHANGELOG.md** under "Unreleased"
4. **Ensure all tests pass**
5. **Link related issues** in PR description
6. **Request review** from maintainers

### PR Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Performance improvement

## Testing
How was this tested?

## Checklist
- [ ] Tests pass
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] Follows code style guidelines
```

---

## Release Process

(For maintainers)

1. Update version in relevant files
2. Update CHANGELOG.md (move Unreleased to version)
3. Create git tag: `git tag -a v1.x.x -m "Release v1.x.x"`
4. Push tag: `git push origin v1.x.x`
5. Create GitHub release with changelog

---

## Code of Conduct

### Our Pledge

We pledge to make participation in our project a harassment-free experience for everyone, regardless of age, body size, disability, ethnicity, gender identity, experience level, nationality, personal appearance, race, religion, or sexual identity.

### Our Standards

**Positive behavior**:
- Using welcoming and inclusive language
- Being respectful of differing viewpoints
- Gracefully accepting constructive criticism
- Focusing on what is best for the community
- Showing empathy towards other community members

**Unacceptable behavior**:
- Trolling, insulting/derogatory comments, personal attacks
- Public or private harassment
- Publishing others' private information without permission
- Other conduct which could reasonably be considered inappropriate

### Enforcement

Project maintainers are responsible for clarifying standards and are expected to take appropriate action in response to unacceptable behavior.

---

## Questions?

- Open an issue for general questions
- Tag `@maintainers` for urgent matters
- Join discussions in GitHub Discussions

---

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for contributing to c-claude!** 🚀
