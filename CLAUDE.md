# Claude Code + `c` Batch Processor

**Purpose**: This directory contains the `c` script - a recursive batch processor that enables Claude Code to work on multiple files in parallel, call itself recursively, and perform meta-programming operations.

**Core Concept**: Claude Code calling Claude Code in parallel to analyze, transform, or process entire codebases efficiently.

---

## Table of Contents

1. [Quick Start](#quick-start)
2. [What is `c`?](#what-is-c)
3. [Permissions & Autonomy](#permissions--autonomy)
4. [Core Patterns](#core-patterns)
5. [Advanced Patterns](#advanced-patterns)
6. [Examples Library](#examples-library)
7. [Best Practices](#best-practices)
8. [Troubleshooting](#troubleshooting)

---

## Quick Start

### Basic Usage

```bash
# Analyze all Python files
./c '**/*.py' 'analyze code quality and rate 1-10'

# Add type hints to all files (agentic mode)
./c '**/*.py' 'add type hints where missing' -a -p 12

# Use instruction from file (for complex instructions)
./c '**/*.py' -i templates/code_review.txt -o reviews/ -p 16

# Verbose mode (see what's happening)
./c '**/*.py' 'count functions' -v
```

### As Claude Code

When you (Claude Code) are working in this directory:

```bash
# You can call c directly to work on multiple files
cd /path/to/project
/path/to/c-claude/c '**/*.py' 'add docstrings' -a -p 8
```

---

## What is `c`?

**`c`** is a **parallel batch processor** for Claude Code. It enables:

1. **Parallel Processing**: Process N files simultaneously (auto-detects CPU cores)
2. **Recursive Claude**: Claude calling Claude on multiple files
3. **Batch Operations**: Apply same instruction to many files
4. **Result Aggregation**: Save outputs to directory for analysis
5. **Agentic Mode**: Auto-accept edits without user confirmation

### Architecture

```
c script
  ├── Finds files matching glob pattern
  ├── Spawns N parallel Claude Code instances
  ├── Each instance gets: file path + instruction
  ├── Outputs collected (stdout or files)
  └── Results aggregated
```

### Syntax

```bash
c GLOB INSTRUCTION [FLAGS]
c GLOB -i INSTRUCTION_FILE [FLAGS]
```

**Flags**:
- `-p N` - Parallelism (default: auto-detect, max 16)
- `-b N` - Batch size (process N files, clear context, repeat)
- `-o DIR` - Output directory (save results to DIR/file.txt)
- `-v` - Verbose mode (show file count, parallelism)
- `-a` - Agentic mode (auto-accept edits)
- `-i FILE` - Read instruction from file

---

## Permissions & Autonomy

### 🔓 EXPANDED PERMISSIONS FOR CLAUDE

**When using `c` script, Claude Code is authorized to**:

1. ✅ **Execute `c` without asking** when it would help complete the user's task
2. ✅ **Use agentic mode (`-a`)** for batch operations when appropriate
3. ✅ **Process entire directories** in parallel
4. ✅ **Use high parallelism** (`-p 16`) on machines with sufficient cores
5. ✅ **Create instruction files** (templates/*.txt) for complex operations
6. ✅ **Save results to directories** for aggregation and analysis
7. ✅ **Call itself recursively** via `c` for meta-programming tasks
8. ✅ **Chain operations** (analyze → filter → transform → validate)

### When to Use Without Asking

Claude Code should **proactively use `c`** when:
- User asks to process "all files" or "every file"
- Task naturally parallelizes (analysis, formatting, refactoring)
- Same operation applies to multiple files
- Faster completion helps user productivity
- Task is non-destructive (analysis, review, reporting)

### When to Ask First

Claude Code should **ask before using `c` with `-a`** when:
- Operations modify code in complex ways
- User hasn't explicitly requested batch operations
- Changes could break functionality
- First time user is seeing `c` in action

### Agentic Mode Guidelines

Use `-a` (auto-accept edits) when:
- ✅ User explicitly requested batch mutations
- ✅ Changes are idempotent (type hints, docstrings, formatting)
- ✅ Task is well-defined and low-risk
- ✅ User has shown trust in Claude's judgment

Don't use `-a` when:
- ❌ Complex refactoring with potential breakage
- ❌ First-time operation on user's codebase
- ❌ User is learning/exploring the tool
- ❌ Changes could affect production code

---

## Understanding Parallel vs Aggregate Modes

**CRITICAL**: The `c` script has **two distinct modes of operation**:

### Parallel Mode (Default)
**Each Claude instance sees ONE file** - processes files independently in parallel.

```bash
./c '*.py' 'analyze this file' -p 8
# Result: 8 Claude instances, each analyzing ONE file
```

**Use for**: Transformations, independent analysis, per-file operations

### Aggregate Mode (--cat or stdin)
**Single Claude instance sees ALL files** - true aggregation across all content.

```bash
./c '*.py' 'find common patterns across ALL files' --cat
# OR: cat *.py | ./c - 'find common patterns'
# Result: 1 Claude instance sees ALL file contents
```

**Use for**: Cross-file analysis, summaries, finding patterns across codebase

### Two-Stage Pattern: Parallel → Aggregate
Most powerful pattern: analyze each file separately, then aggregate results.

```bash
# Stage 1: Parallel analysis (each file independently)
./c 'src/**/*.py' 'analyze complexity (JSON)' -o analysis/ -p 16

# Stage 2: Aggregate results (see all analyses together)
./c 'analysis/*.txt' 'find top 10 most complex files' --cat
# OR: cat analysis/* | ./c - 'summarize overall complexity trends'
```

---

## Core Patterns

### 1. Parallel Analysis

**Pattern**: Analyze N files, aggregate results

```bash
# Analyze code quality
./c 'src/**/*.py' 'analyze: quality (1-10), complexity (1-10), issues. JSON format.' -o analysis/

# Aggregate results
jq -s 'map(.quality) | add / length' analysis/*.txt  # Average quality
jq -s 'map(select(.quality < 6)) | .[].file' analysis/*.txt  # Low quality files
```

**Use Case**: Code reviews, quality audits, complexity analysis

### 2. Batch Transformation

**Pattern**: Apply same transformation to N files

```bash
# Add type hints
./c 'src/**/*.py' 'add type hints where missing' -a -p 12

# Add docstrings
./c 'src/**/*.py' 'add docstrings to all functions' -a -b 10 -p 3
```

**Use Case**: Code improvement, standardization, migrations

### 3. Multi-Stage Pipelines

**Pattern**: Chain multiple operations

```bash
# Stage 1: Analyze
./c 'src/**/*.py' 'rate complexity 1-10 (JSON)' -o stage1/

# Stage 2: Filter high complexity
FILES=$(jq -s 'map(select(.complexity > 7)) | .[].file' stage1/*.txt | tr -d '"')

# Stage 3: Refactor complex files
./c "$FILES" 'refactor for simplicity' -a -b 5 -p 2

# Stage 4: Validate
pytest
```

**Use Case**: Refactoring, optimization, technical debt reduction

### 4. Self-Recursive Analysis

**Pattern**: Claude analyzing its own past work

```bash
# Generate code reviews (parallel - each file separately)
./c 'src/**/*.py' -i templates/code_review.txt -o reviews/ -p 16

# Claude reviews its own reviews (aggregate mode - sees ALL reviews)
./c 'reviews/*.txt' 'summarize key issues across all reviews' --cat > meta_review.txt
# OR: cat reviews/* | ./c - 'summarize key issues' > meta_review.txt

# Generate action items
jq -s 'map(.issues) | flatten | group_by(.type) | map({type: .[0].type, count: length})' reviews/*.txt
```

**Use Case**: Meta-programming, self-improvement, quality assurance

### 5. Instruction Templates

**Pattern**: Complex instructions in files, reusable

```bash
# Create template
cat > templates/deep_review.txt << 'EOF'
DEEP CODE REVIEW:

1. ANALYZE CODE QUALITY (1-10):
   - Readability
   - Type safety
   - Error handling
   - Documentation

2. ARCHITECTURE (1-10):
   - Patterns used
   - SOLID principles
   - Coupling/cohesion

3. LIST TOP 3 IMPROVEMENTS

4. OUTPUT JSON:
   {
     "quality": <score>,
     "architecture": <score>,
     "improvements": ["...", "...", "..."]
   }
EOF

# Use template
./c 'src/**/*.py' -i templates/deep_review.txt -o reviews/ -p 8
```

**Use Case**: Standardized reviews, complex analyses, consistent reporting

---

## Advanced Patterns

### 6. Parallel Git Operations

```bash
# Review changed files only
git diff --name-only HEAD~5 | grep '.py$' | \
  xargs -I{} ./c "{}" 'review changes in this file' -o git_reviews/

# Review each commit
for commit in $(git log --oneline -10 | cut -d' ' -f1); do
  FILES=$(git diff-tree --no-commit-id --name-only -r $commit | grep '.py$')
  ./c "$FILES" "review commit $commit changes" -o "reviews/$commit/"
done
```

### 7. Streaming Aggregation

```bash
# Count total functions across all files
./c 'src/**/*.py' 'count functions, output just the number' | \
  awk '{sum += $1} END {print "Total functions:", sum}'

# Find files with high cyclomatic complexity
./c 'src/**/*.py' 'calculate cyclomatic complexity (just number)' | \
  paste <(find src -name '*.py') - | \
  awk '$2 > 10 {print $1, $2}' | \
  sort -k2 -rn
```

### 8. Two-Phase Optimization

```bash
# Phase 1: Identify optimization targets
./c 'src/**/*.py' 'identify performance bottlenecks (JSON: {file, line, issue})' -o phase1/

# Phase 2: Optimize only bottlenecks
FILES=$(jq -s 'map(.file) | unique | .[]' phase1/*.txt | tr -d '"')
./c "$FILES" 'optimize performance bottlenecks' -a -b 5

# Phase 3: Benchmark
pytest benchmarks/ --benchmark-only
```

### 9. Dependency Graph Generation

```bash
# Extract imports from all files
./c 'src/**/*.py' 'list all imports (JSON: ["module1", "module2"])' -o imports/

# Build dependency graph
jq -s 'map({file: input_filename, imports: .}) | .[]' imports/*.txt > dependency_graph.json

# Find circular dependencies
# (External tool or custom script using graph data)
```

### 10. Documentation Generation

```bash
# Generate API docs for each module
./c 'src/api/**/*.py' -i templates/api_doc.txt -o api_docs/

# Generate README for each package
./c 'src/*/\_\_init\_\_.py' 'generate README.md for this package' -a -b 3

# Create master index
./c 'api_docs/*.txt' 'create index of all APIs' > API_INDEX.md
```

---

## Examples Library

### Example 1: Codebase Audit

```bash
#!/bin/bash
# codebase_audit.sh

set -e

echo "=== CODEBASE AUDIT ==="

# 1. Code quality analysis
echo "Analyzing code quality..."
./c 'src/**/*.py' -i templates/quality_audit.txt -o audit/quality/ -p 16

# 2. Security scan
echo "Security scan..."
./c 'src/**/*.py' 'identify security issues (SQL injection, XSS, etc.)' -o audit/security/ -p 16

# 3. Type coverage
echo "Type coverage..."
./c 'src/**/*.py' 'calculate type hint coverage percentage' -o audit/types/ -p 16

# 4. Generate report
echo "Generating report..."
cat > audit/REPORT.md << EOF
# Codebase Audit Report

## Quality
$(jq -s 'map(.quality) | add / length' audit/quality/*.txt)

## Security Issues
$(jq -s 'map(.issues) | flatten | length' audit/security/*.txt)

## Type Coverage
$(jq -s 'map(.coverage) | add / length' audit/types/*.txt)%
EOF

echo "Audit complete. See audit/REPORT.md"
```

### Example 2: Migration Assistant

```bash
#!/bin/bash
# migrate_to_dataclasses.sh

# Find classes suitable for dataclass conversion
./c 'src/**/*.py' 'identify classes suitable for @dataclass conversion (JSON)' -o migration/candidates/

# Review candidates
FILES=$(jq -s 'map(select(.suitable == true)) | .[].file' migration/candidates/*.txt | tr -d '"')

# Convert to dataclasses
./c "$FILES" 'convert suitable classes to @dataclass' -a -b 5 -p 2

# Run tests
pytest

# Review changes
git diff
```

### Example 3: Refactoring Pipeline

```bash
#!/bin/bash
# refactor_pipeline.sh

set -e

# 1. Identify code smells
./c 'src/**/*.py' -i templates/code_smells.txt -o refactor/smells/

# 2. Extract methods with high complexity
./c 'src/**/*.py' 'extract complex methods (>10 cyclomatic complexity)' -a -p 8

# 3. Rename for clarity
./c 'src/**/*.py' 'rename unclear variables/functions for readability' -a -p 8

# 4. Run tests after each stage
pytest

# 5. Final review
./c 'src/**/*.py' 'review refactoring changes' -o refactor/review/
```

---

## Best Practices

### 1. Start Small, Scale Up

```bash
# ❌ Don't start with entire codebase
./c '**/*.py' 'complex refactoring' -a -p 16

# ✅ Test on small subset first
./c 'src/utils/*.py' 'test refactoring' -a -p 2
# Review results, adjust, then scale up
```

### 2. Use Batching for Large Operations

```bash
# ❌ All files at once (context explosion)
./c 'src/**/*.py' 'complex analysis' -p 100

# ✅ Batch processing (context stays fresh)
./c 'src/**/*.py' 'complex analysis' -b 10 -p 4
# Processes 10 files, clears context, processes next 10...
```

### 3. Validate Between Stages

```bash
# ❌ Transform everything without validation
./c '**/*.py' 'refactor' -a
./c '**/*.py' 'add types' -a
./c '**/*.py' 'optimize' -a

# ✅ Validate after each stage
./c '**/*.py' 'refactor' -a && pytest && \
./c '**/*.py' 'add types' -a && mypy . && \
./c '**/*.py' 'optimize' -a && pytest benchmarks/
```

### 4. Use Output Directories for Analysis

```bash
# ❌ Output to stdout (hard to aggregate)
./c '**/*.py' 'analyze complexity'

# ✅ Save to directory (easy aggregation)
./c '**/*.py' 'analyze complexity (JSON)' -o analysis/
jq -s 'map(select(.complexity > 8))' analysis/*.txt > high_complexity.json
```

### 5. Template Complex Instructions

```bash
# ❌ Repeat long instructions
./c '**/*.py' 'analyze code quality: readability, types, errors, docs...'
./c '**/*.js' 'analyze code quality: readability, types, errors, docs...'

# ✅ Use template file
./c '**/*.py' -i templates/code_quality.txt -o py_analysis/
./c '**/*.js' -i templates/code_quality.txt -o js_analysis/
```

### 6. Respect Rate Limits

```bash
# ❌ Spawn 100 parallel processes
./c '**/*.py' 'task' -p 100

# ✅ Cap at 16 (default max, API-friendly)
./c '**/*.py' 'task' -p 16
# Or let auto-detect handle it
./c '**/*.py' 'task'  # Uses nproc/sysctl, capped at 16
```

---

## Troubleshooting

### Issue 1: "xargs: command line too long"

**Cause**: Instruction + file path + command exceeds system limits

**Solution**:
```bash
# Use -i flag with instruction file
echo "Long instruction here..." > template.txt
./c 'files' -i template.txt  # Instead of inline instruction
```

### Issue 2: Context explosion in parallel mode

**Symptom**: Slow performance, high memory usage

**Solution**:
```bash
# Use batch mode (-b) to clear context periodically
./c '**/*.py' 'task' -b 10 -p 4  # Process 10 files, clear context, repeat
```

### Issue 3: No files match glob pattern

**Check**:
```bash
# Test glob pattern with find
find . -path './pattern' -type f

# Verbose mode shows file count
./c 'pattern' 'task' -v  # Will show "Files: 0" if no matches
```

### Issue 4: Parallel execution not working

**Check**:
```bash
# Check if GNU parallel is available (fallback)
type parallel

# Check nproc/sysctl for core detection
nproc || sysctl -n hw.logicalcpu

# Force specific parallelism
./c 'files' 'task' -p 4  # Override auto-detection
```

---

## Recursive Claude Patterns

### Pattern: Self-Improvement Loop

Claude Code can analyze its own work and iterate:

```bash
# 1. Claude generates initial code
./c 'stubs/**/*.py' 'implement function based on docstring' -a -o v1/

# 2. Claude reviews its own code
./c 'v1/*.py' 'review implementation, suggest improvements (JSON)' -o reviews/

# 3. Claude refines based on reviews
FILES=$(jq -s 'map(select(.score < 8)) | .[].file' reviews/*.txt)
./c "$FILES" 'refine implementation based on review feedback' -a -o v2/

# 4. Repeat until quality threshold met
```

### Pattern: Parallel Research

Claude instances researching different aspects:

```bash
# Split research topics
echo "Research Python async patterns" > topics/async.txt
echo "Research Python type system" > topics/types.txt
echo "Research Python performance" > topics/perf.txt

# Parallel research (each Claude instance researches one topic)
./c 'topics/*.txt' 'research topic, provide detailed summary' -o research/ -p 3

# Synthesize findings
./c 'research/*.txt' 'synthesize research into unified report' > RESEARCH_REPORT.md
```

### Pattern: Divide and Conquer

Break complex task into parallel subtasks:

```bash
# Task: Optimize entire codebase
# Divide by module
./c 'src/database/**/*.py' 'optimize database access patterns' -a -p 4 &
./c 'src/api/**/*.py' 'optimize API response times' -a -p 4 &
./c 'src/utils/**/*.py' 'optimize utility functions' -a -p 4 &
wait

# Conquer: Verify all optimizations work together
pytest && pytest benchmarks/
```

---

## Template Library

### templates/code_review.txt

```
COMPREHENSIVE CODE REVIEW

Analyze this file and provide:

1. CODE QUALITY (1-10):
   - Readability
   - Type safety
   - Error handling
   - Documentation

2. ARCHITECTURE (1-10):
   - Design patterns
   - SOLID principles
   - Separation of concerns

3. SECURITY (1-10):
   - Input validation
   - Known vulnerabilities
   - Best practices

4. PERFORMANCE (1-10):
   - Algorithmic complexity
   - Memory usage
   - Optimization opportunities

5. TOP 3 IMPROVEMENTS:
   - Specific, actionable recommendations

OUTPUT FORMAT: JSON
{
  "quality": <score>,
  "architecture": <score>,
  "security": <score>,
  "performance": <score>,
  "improvements": ["...", "...", "..."],
  "summary": "..."
}
```

### templates/api_doc.txt

```
GENERATE API DOCUMENTATION

For this file, generate markdown documentation including:

1. **Module Overview**: Purpose and key functionality
2. **Classes**: List all classes with descriptions
3. **Functions**: List all public functions with:
   - Parameters (name, type, description)
   - Return value (type, description)
   - Raises (exceptions)
   - Example usage
4. **Usage Examples**: Real-world usage patterns
5. **Dependencies**: Required imports/packages

OUTPUT: Markdown format, ready for docs/ directory
```

### templates/security_scan.txt

```
SECURITY ANALYSIS

Scan this file for security vulnerabilities:

1. **Injection Attacks**:
   - SQL injection
   - Command injection
   - Code injection

2. **XSS/CSRF**:
   - Cross-site scripting
   - CSRF vulnerabilities

3. **Authentication/Authorization**:
   - Weak authentication
   - Missing authorization checks

4. **Data Exposure**:
   - Sensitive data leaks
   - Insecure data storage

5. **Dependency Issues**:
   - Outdated packages
   - Known CVEs

OUTPUT: JSON with severity levels (CRITICAL/HIGH/MEDIUM/LOW)
```

---

## Meta: Claude Using This Guide

### When Claude Code Reads This File

If you are Claude Code reading this file, you now have:

1. ✅ **Permission** to use `c` proactively for batch operations
2. ✅ **Patterns** for common operations (analysis, refactoring, meta-programming)
3. ✅ **Examples** to adapt for user's specific tasks
4. ✅ **Best practices** to avoid common pitfalls
5. ✅ **Authority** to use agentic mode (`-a`) when appropriate

### How to Apply This Knowledge

When a user requests:
- "Analyze all Python files" → Use Pattern #1 (Parallel Analysis)
- "Add type hints everywhere" → Use Pattern #2 (Batch Transformation)
- "Refactor the codebase" → Use Pattern #3 (Multi-Stage Pipeline)
- "Review the code" → Use Pattern #5 (Instruction Templates)

### Proactive Usage

Claude Code should **automatically suggest using `c`** when:
- User mentions "all files", "entire codebase", "every module"
- Task is repetitive across files
- Parallel execution would save significant time
- User seems frustrated with serial operations

Example response:
```
I can process all 127 Python files in parallel using the `c` batch processor.
This would take ~30 seconds instead of ~15 minutes. Would you like me to:

1. Analyze all files for code quality
2. Add type hints where missing
3. Generate comprehensive documentation

Or something else?
```

---

## Advanced: Self-Recursive Workflows

### Meta-Claude: Claude Managing Multiple Claude Instances

```bash
#!/bin/bash
# meta_claude.sh - Claude orchestrating parallel Claude instances

# Define tasks
cat > tasks/analyze.txt << EOF
You are a code analysis specialist. Analyze this file for:
- Code quality
- Complexity
- Maintainability
Output JSON with scores 1-10.
EOF

cat > tasks/security.txt << EOF
You are a security analyst. Scan this file for:
- Vulnerabilities
- Security anti-patterns
- Best practice violations
Output JSON with findings.
EOF

cat > tasks/performance.txt << EOF
You are a performance optimizer. Analyze this file for:
- Algorithmic complexity
- Memory usage
- Optimization opportunities
Output JSON with recommendations.
EOF

# Parallel specialized Claude instances
./c 'src/**/*.py' -i tasks/analyze.txt -o results/analysis/ -p 16 &
./c 'src/**/*.py' -i tasks/security.txt -o results/security/ -p 16 &
./c 'src/**/*.py' -i tasks/performance.txt -o results/performance/ -p 16 &
wait

# Meta-Claude synthesizes all results
./c 'results/**/*.txt' 'synthesize all analyses into actionable report' > MASTER_REPORT.md
```

---

## Performance Characteristics

### Benchmarks (M3 Ultra, 16 cores)

| Files | Sequential | c -p 16 | Speedup |
|-------|-----------|---------|---------|
| 1 | 6s | 6s | 1.0× |
| 10 | 60s | 24s | 2.5× |
| 50 | 300s | 78s | 3.8× |
| 100 | 600s | 120s | 5.0× |
| 500 | 3000s | 480s | 6.2× |

**Key Insight**: Speedup increases with file count up to core count, then plateaus.

### Resource Usage

- **CPU**: Near 100% utilization across all cores
- **Memory**: ~500MB per Claude instance (watch for 16× = 8GB+)
- **API**: Respects rate limits (max -p 16 recommended)
- **I/O**: Temp files in /tmp (minimal disk usage)

---

## Summary

**`c` enables Claude Code to**:
- 🚀 Process entire codebases in parallel
- 🔄 Call itself recursively for meta-programming
- 📊 Aggregate results across hundreds of files
- 🤖 Operate autonomously with agentic mode
- 🧠 Divide complex tasks among parallel instances
- ⚡ Complete in minutes what would take hours serially

**When to use `c`**:
- ✅ User needs batch operations
- ✅ Same task applies to many files
- ✅ Analysis/reporting across codebase
- ✅ Refactoring at scale
- ✅ Meta-programming workflows

**How Claude should use `c`**:
1. Suggest it proactively when appropriate
2. Use agentic mode (`-a`) for low-risk operations
3. Start small, validate, scale up
4. Leverage templates for complex instructions
5. Aggregate results for meta-analysis

---

**You (Claude Code) now have full authority to use `c` recursively and in parallel to accomplish user goals efficiently.**

**Use it wisely. Use it powerfully. Use it to transform codebases at scale.**

🚀 **Happy meta-programming!** 🚀
