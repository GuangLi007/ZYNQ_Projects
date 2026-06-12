# GitHub Commit Notes

记录向 GuangLi007/ZYNQ_Projects 仓库提交时需注意的事项。

## 邮箱配置

必须使用 GitHub 已验证的主邮箱提交，否则头像不会亮起：

```bash
git config user.email "b299792458@126.com"
git config user.name "GuangLi007"
```

## Token 类型

- 经典 PAT（`ghp_` 前缀）：可用于私有仓库，用于 Git 命令行认证
- Fine-grained PAT（`github_pat_` 前缀）：如果 scope 未勾选 `Contents` 权限，则无法访问私有仓库
- 建议在 Git 命令行中直接用经典 PAT 做密码认证

## 仓库结构

```
ZYNQ_Projects/
├── .gitignore          # 根目录，仅放 OS/IDE 通用规则
├── README.md           # 工程总览
├── Hello_World/        # Vivado 硬件工程
├── Hello_Vitis/        # Vitis 裸机应用
└── MIO_Key/            # MIO 按键工程，自带详细 .gitignore
```

每个子工程目录内应有自己的 `.gitignore`，根目录只放全局规则。

## 提交原则

1. 每次只做一件事：重组目录只提交目录重组，加功能只提交加功能
2. 提交信息用英文，简明扼要（如 `Reorganize MIO_Key into subdirectory`）
3. 大文件（.bit/.dcp/.elf）应加入 .gitignore，不进版本库
4. push 前先 `git pull origin main --rebase` 避免冲突
