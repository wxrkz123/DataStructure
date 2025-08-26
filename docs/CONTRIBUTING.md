<div align="right">
  
  [English](./CONTRIBUTING_EN.md) | **简体中文**
  
</div>

# 🤝 贡献指南

首先，感谢你考虑为 **数据结构项目** 做出贡献！正是像你这样的人让开源社区变得更加美好。

## 📋 目录

- [行为准则](#行为准则)
- [我该如何贡献？](#我该如何贡献)
  - [报告 Bug](#-报告-bug)
  - [建议新功能](#-建议新功能)
  - [提交代码](#-提交代码)
  - [改进文档](#-改进文档)
- [开发流程](#开发流程)
- [代码规范](#代码规范)
- [提交信息规范](#提交信息规范)
- [Pull Request 流程](#pull-request-流程)

## 行为准则

本项目采用开源社区行为准则。参与项目即表示你同意遵守其条款。请友善、包容地对待每一位贡献者。

## 我该如何贡献？

### 🐛 报告 Bug

发现 Bug 是对项目的贡献！如果你发现了问题，请：

1. **确认 Bug 尚未被报告** - 先搜索 [Issues](https://github.com/Frank-Code-Show/DataStructure/issues)
2. **创建详细的 Bug 报告**，包含：
   - 清晰的标题和描述
   - 重现步骤
   - 预期行为 vs 实际行为
   - 截图（如果适用）
   - 环境信息（操作系统、编译器版本等）

**Bug 报告模板：**
```markdown
**描述问题**
简明扼要地描述问题是什么。

**重现步骤**
1. 进入 '...'
2. 点击 '....'
3. 滚动到 '....'
4. 出现错误

**预期行为**
描述你期望发生什么。

**截图**
如果适用，请添加截图。

**环境信息：**
- OS: [例如 Windows 10]
- 编译器: [例如 gcc 9.3.0]
- 版本: [例如 commit hash]
```

### 💡 建议新功能

我们欢迎新想法！请通过 Issue 提交功能请求：

1. **明确的功能描述** - 这个功能是什么？
2. **使用场景** - 为什么需要这个功能？
3. **可能的实现方案** - 你认为如何实现？
4. **替代方案** - 是否考虑过其他解决方案？

### 🔧 提交代码

想要提交代码？太棒了！请遵循以下步骤：

#### 1. Fork 和 Clone
```bash
# Fork 项目到你的 GitHub 账号
# 然后 clone 到本地
git clone https://github.com/你的用户名/DataStructure.git
cd DataStructure
```

#### 2. 创建分支
```bash
# 基于最新的 main 分支创建新分支
git checkout -b feature/你的功能名称
# 或
git checkout -b fix/修复的问题
```

#### 3. 进行修改
- 编写清晰、可维护的代码
- 添加必要的注释
- 更新相关文档
- 添加测试（如果适用）

#### 4. 提交更改
```bash
git add .
git commit -m "feat: 添加某某功能"
```

#### 5. 推送到 GitHub
```bash
git push origin feature/你的功能名称
```

#### 6. 创建 Pull Request
在 GitHub 上创建 PR，填写 PR 模板

### 📝 改进文档

文档同样重要！你可以：
- 修正错别字或语法错误
- 改进示例代码
- 添加更多解释说明
- 翻译文档

## 开发流程

### 项目结构
```
DataStructure/
├── dsc/              # HTML 动画演示
├── dsc-code/         # C 语言实现
│   ├── include/      # 头文件
│   ├── src/          # 源代码
│   └── tests/        # 测试文件
└── docs/             # 文档
```

### 编译和测试
```bash
# 编译代码
cd dsc-code
make all

# 运行测试
make test

# 清理编译文件
make clean
```

## 代码规范

### C 语言规范

1. **命名约定**
   - 函数名：`camelCase` 或 `snake_case`（保持一致）
   - 常量：`UPPER_SNAKE_CASE`
   - 结构体：`PascalCase`

2. **格式化**
   - 缩进：4 个空格
   - 最大行长：80 字符
   - 大括号：K&R 风格

3. **注释**
   ```c
   /* 多行注释
    * 用于函数说明
    */
   
   // 单行注释用于代码内部说明
   ```

4. **示例代码**
   ```c
   /**
    * 创建新节点
    * @param value 节点值
    * @return 指向新节点的指针
    */
   Node* createNode(int value) {
       Node* newNode = (Node*)malloc(sizeof(Node));
       if (newNode == NULL) {
           fprintf(stderr, "内存分配失败\n");
           return NULL;
       }
       newNode->data = value;
       newNode->next = NULL;
       return newNode;
   }
   ```

### HTML/CSS 规范

- 使用语义化 HTML5 标签
- CSS 采用 BEM 命名规范
- 确保动画流畅（60fps）

## 提交信息规范

采用 [Conventional Commits](https://www.conventionalcommits.org/) 规范：

```
<类型>(<范围>): <主题>

<正文>

<页脚>
```

**类型：**
- `feat`: 新功能
- `fix`: 修复 Bug
- `docs`: 文档更新
- `style`: 代码格式（不影响功能）
- `refactor`: 重构
- `test`: 添加测试
- `chore`: 构建过程或辅助工具的变动

**示例：**
```
feat(linkedlist): 添加反转链表功能

实现了 reverseList() 函数，可以原地反转单向链表。
时间复杂度 O(n)，空间复杂度 O(1)。

Closes #123
```

## Pull Request 流程

1. **PR 前检查清单**
   - [ ] 代码通过所有测试
   - [ ] 遵循代码规范
   - [ ] 更新了相关文档
   - [ ] 提交信息清晰规范
   - [ ] PR 描述完整

2. **PR 模板**
   ```markdown
   ## 描述
   简要描述这个 PR 的改动

   ## 改动类型
   - [ ] Bug 修复
   - [ ] 新功能
   - [ ] 破坏性变更
   - [ ] 文档更新

   ## 测试
   描述你如何测试这些改动

   ## 截图（如果适用）
   添加截图帮助解释改动

   ## 相关 Issue
   Closes #(issue 编号)
   ```

3. **Review 流程**
   - 至少需要 1 位维护者的批准
   - 所有 CI 检查必须通过
   - 解决所有 review 意见

## 🎯 小贴士

- **先讨论，后编码** - 对于大的改动，先开 Issue 讨论
- **保持简单** - 每个 PR 只做一件事
- **写好测试** - 新功能需要配套测试
- **耐心等待** - 维护者可能需要时间 review

## 📮 联系方式

- **B站**: [@Frank](https://space.bilibili.com/19658621)
- **GitHub Issues**: [项目 Issues](https://github.com/Frank-Code-Show/DataStructure/issues)

---

<div align="center">
  
  再次感谢你的贡献！🎉
  
  **[⬆ 回到顶部](#-贡献指南)**
  
</div>
