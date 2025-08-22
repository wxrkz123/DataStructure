<div align="right">
  
  [English](./README_EN.md) | **简体中文**
  
</div>

<div align="center">
  
  # 📊 数据结构项目
  
  <p align="center">
    <strong>直面数据结构，可视化学习之旅</strong>
  </p>
  
  <p align="center">
    <img src="https://img.shields.io/badge/language-C17-blue.svg" alt="Language">
    <img src="https://img.shields.io/badge/platform-Cross--platform-lightgrey.svg" alt="Platform">
    <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License">
    <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg" alt="PRs Welcome">
  </p>
  
  <p align="center">
    <a href="#✨-特性">特性</a> •
    <a href="#🚀-快速开始">快速开始</a> •
    <a href="#📁-项目结构">项目结构</a> •
    <a href="#🤝-贡献">贡献</a> •
    <a href="#📄-许可证">许可证</a>
  </p>
  
</div>

---

## 🎯 简介

一个优雅的数据结构学习项目，配合 B站 UP主 [Frank](https://space.bilibili.com/19658621) 的《直面数据结构》课程，让复杂概念变得简单易懂。

## ✨ 特性

🎨 **可视化动画** - 直观展示数据结构操作过程  
📚 **完整实现** - 涵盖链表、栈、队列、树、图等经典结构  
💡 **详细注释** - 每行代码都有清晰解释  
🔧 **工具集成** - 内置字符编码转换工具  
🎓 **教学优先** - 专为学习者设计  

## 🚀 快速开始

### 克隆项目

```bash
git clone https://github.com/Frank-Code-Show/DataStructure.git
cd DataStructure
```

### 查看动画演示

```bash
# 打开任意 HTML 文件
open dsc/demo.html  # macOS
start dsc/demo.html # Windows
```

### 编译 C 代码

```bash
cd dsc-code
gcc -std=c17 -o demo main.c
./demo
```

## 📁 项目结构

```
DataStructure/
│
├── 📂 dsc/                 # 可视化动画演示
│   └── *.html             # HTML 动画文件
│
├── 📂 dsc-code/           # C语言实现
│   ├── linkedlist.c      # 链表
│   ├── stack.c           # 栈
│   ├── queue.c           # 队列
│   └── ...               # 更多数据结构
│
└── 🔧 tools/              # 实用工具
    └── convert_encoding.py # 编码转换工具
```

## 💻 代码示例

```c
// 简洁优雅的链表实现
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

## 🤝 贡献

我们欢迎所有形式的贡献！查看 [贡献指南](CONTRIBUTING.md) 了解如何：

- 🐛 报告问题
- 💡 提出新功能
- 📝 改进文档
- 🔧 提交代码

## 👨‍💻 作者

**Frank** - *B站创作者* - [@Frank](https://space.bilibili.com/19658621)

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情

---

<div align="center">
  
  **[⬆ 回到顶部](#-数据结构项目)**
  
  如果这个项目对你有帮助，请给一个 ⭐️！
  
</div>
