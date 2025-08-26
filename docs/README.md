<div align="right">
  
  **English** | [简体中文](./docs/README_ZH.md)
  
</div>

<div align="center">
  
  # 📊 Data Structure Project
  
  <p align="center">
    <strong>Face Data Structures, A Visual Learning Journey</strong>
  </p>
  
  <p align="center">
    <img src="https://img.shields.io/badge/language-C17-blue.svg" alt="Language">
    <img src="https://img.shields.io/badge/platform-Cross--platform-lightgrey.svg" alt="Platform">
    <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License">
    <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg" alt="PRs Welcome">
  </p>
  
  <p align="center">
    <a href="#-features">Features</a> •
    <a href="#-quick-start">Quick Start</a> •
    <a href="#-project-structure">Structure</a> •
    <a href="#-contributing">Contributing</a> •
    <a href="#-license">License</a>
  </p>
  
</div>

---

## 🎯 Introduction

An elegant data structure learning project, created alongside [Frank](https://space.bilibili.com/19658621)'s "Facing Data Structures" course, making complex concepts simple and understandable.

## ✨ Features

🎨 **Visual Animations** - Intuitive demonstrations of data structure operations  
📚 **Complete Implementations** - Covers linked lists, stacks, queues, trees, graphs, and more  
💡 **Detailed Comments** - Clear explanations for every line of code  
🔧 **Integrated Tools** - Built-in character encoding conversion utilities  
🎓 **Education First** - Designed specifically for learners  

## 🚀 Quick Start

### Clone the Repository

```bash
git clone https://github.com/Frank-Code-Show/DataStructure.git
cd DataStructure
```

### View Animations

```bash
# Open any HTML file
open dsc/demo.html  # macOS
start dsc/demo.html # Windows
```

### Compile C Code

```bash
cd dsc-code
gcc -std=c17 -o demo main.c
./demo
```

## 📁 Project Structure

```
DataStructure/
│
├── 📂 dsc/                 # Visual animations
│   └── *.html             # HTML animation files
│
├── 📂 dsc-code/           # C implementations
│   ├── linkedlist.c      # Linked list
│   ├── stack.c           # Stack
│   ├── queue.c           # Queue
│   └── ...               # More structures
│
└── 🔧 tools/              # Utilities
    └── convert_encoding.py # Encoding converter
```

## 💻 Code Example

```c
// Clean and elegant linked list implementation
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}
```

## 🤝 Contributing

We welcome all forms of contributions! Check out the [Contributing Guide](CONTRIBUTING_EN.md) to learn how to:

- 🐛 Report issues
- 💡 Suggest new features
- 📝 Improve documentation
- 🔧 Submit code

## 👨‍💻 Author

**Frank** - *Bilibili Creator* - [@Frank](https://space.bilibili.com/19658621)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

---

<div align="center">
  
  **[⬆ Back to Top](#-data-structure-project)**
  
  If this project helps you, please give it a ⭐️!
  
</div>
