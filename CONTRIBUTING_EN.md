<div align="right">
  
  **English** | [简体中文](./CONTRIBUTING.md)
  
</div>

# 🤝 Contributing Guide

First off, thank you for considering contributing to the **Data Structure Project**! It's people like you that make the open source community such a great place.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
  - [Reporting Bugs](#-reporting-bugs)
  - [Suggesting Features](#-suggesting-features)
  - [Submitting Code](#-submitting-code)
  - [Improving Documentation](#-improving-documentation)
- [Development Process](#development-process)
- [Code Standards](#code-standards)
- [Commit Message Guidelines](#commit-message-guidelines)
- [Pull Request Process](#pull-request-process)

## Code of Conduct

This project adopts the open source community code of conduct. By participating, you are expected to uphold this code. Please treat every contributor with kindness and respect.

## How Can I Contribute?

### 🐛 Reporting Bugs

Finding bugs is a contribution! If you find an issue, please:

1. **Check if the bug has been reported** - Search existing [Issues](https://github.com/Frank-Code-Show/DataStructure/issues)
2. **Create a detailed bug report** including:
   - Clear title and description
   - Steps to reproduce
   - Expected vs actual behavior
   - Screenshots (if applicable)
   - Environment info (OS, compiler version, etc.)

**Bug Report Template:**
```markdown
**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
Describe what you expected to happen.

**Screenshots**
If applicable, add screenshots.

**Environment:**
- OS: [e.g. Windows 10]
- Compiler: [e.g. gcc 9.3.0]
- Version: [e.g. commit hash]
```

### 💡 Suggesting Features

We welcome new ideas! Please submit feature requests via Issues:

1. **Clear feature description** - What is this feature?
2. **Use case** - Why is this feature needed?
3. **Possible implementation** - How do you think it should work?
4. **Alternatives** - Have you considered other solutions?

### 🔧 Submitting Code

Want to submit code? Awesome! Please follow these steps:

#### 1. Fork and Clone
```bash
# Fork the project to your GitHub account
# Then clone locally
git clone https://github.com/your-username/DataStructure.git
cd DataStructure
```

#### 2. Create a Branch
```bash
# Create a new branch from the latest main
git checkout -b feature/your-feature-name
# or
git checkout -b fix/issue-to-fix
```

#### 3. Make Changes
- Write clean, maintainable code
- Add necessary comments
- Update related documentation
- Add tests (if applicable)

#### 4. Commit Changes
```bash
git add .
git commit -m "feat: add new feature"
```

#### 5. Push to GitHub
```bash
git push origin feature/your-feature-name
```

#### 6. Create Pull Request
Create a PR on GitHub and fill out the PR template

### 📝 Improving Documentation

Documentation is equally important! You can:
- Fix typos or grammar errors
- Improve code examples
- Add more explanations
- Translate documentation

## Development Process

### Project Structure
```
DataStructure/
├── dsc/              # HTML animations
├── dsc-code/         # C implementations
│   ├── include/      # Header files
│   ├── src/          # Source code
│   └── tests/        # Test files
└── docs/             # Documentation
```

### Build and Test
```bash
# Build the code
cd dsc-code
make all

# Run tests
make test

# Clean build files
make clean
```

## Code Standards

### C Language Standards

1. **Naming Conventions**
   - Functions: `camelCase` or `snake_case` (be consistent)
   - Constants: `UPPER_SNAKE_CASE`
   - Structs: `PascalCase`

2. **Formatting**
   - Indentation: 4 spaces
   - Max line length: 80 characters
   - Braces: K&R style

3. **Comments**
   ```c
   /* Multi-line comments
    * for function descriptions
    */
   
   // Single-line comments for inline explanations
   ```

4. **Example Code**
   ```c
   /**
    * Create a new node
    * @param value The node value
    * @return Pointer to the new node
    */
   Node* createNode(int value) {
       Node* newNode = (Node*)malloc(sizeof(Node));
       if (newNode == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
           return NULL;
       }
       newNode->data = value;
       newNode->next = NULL;
       return newNode;
   }
   ```

### HTML/CSS Standards

- Use semantic HTML5 tags
- Follow BEM naming convention for CSS
- Ensure smooth animations (60fps)

## Commit Message Guidelines

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation update
- `style`: Code formatting (no functional changes)
- `refactor`: Code refactoring
- `test`: Adding tests
- `chore`: Changes to build process or tools

**Example:**
```
feat(linkedlist): add reverse linked list function

Implemented reverseList() function for in-place reversal of singly linked list.
Time complexity O(n), space complexity O(1).

Closes #123
```

## Pull Request Process

1. **PR Checklist**
   - [ ] Code passes all tests
   - [ ] Follows code standards
   - [ ] Documentation updated
   - [ ] Clear commit messages
   - [ ] Complete PR description

2. **PR Template**
   ```markdown
   ## Description
   Brief description of the changes in this PR

   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Breaking change
   - [ ] Documentation update

   ## Testing
   Describe how you tested these changes

   ## Screenshots (if applicable)
   Add screenshots to help explain changes

   ## Related Issues
   Closes #(issue number)
   ```

3. **Review Process**
   - Requires at least 1 maintainer approval
   - All CI checks must pass
   - Address all review comments

## 🎯 Tips

- **Discuss before coding** - For major changes, open an issue first
- **Keep it simple** - One PR should do one thing
- **Write tests** - New features need tests
- **Be patient** - Maintainers may need time to review

## 📮 Contact

- **Bilibili**: [@Frank](https://space.bilibili.com/19658621)
- **GitHub Issues**: [Project Issues](https://github.com/Frank-Code-Show/DataStructure/issues)

---

<div align="center">
  
  Thank you again for your contribution! 🎉
  
  **[⬆ Back to Top](#-contributing-guide)**
  
</div>
