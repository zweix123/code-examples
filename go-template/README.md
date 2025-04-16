# go-template

## dev tool

- format:

  > go 标准工具链包括格式化工具 gofmt, 但是这里使用第三方的

  - gofumpt: 更严格的格式化工具
  - goimports: 自动整理导入语句

- lint: 推荐 golangci-lint, 集成多个 Linter, 需要配置文件`.golangci.yml`

- 其他:
  - govulncheck: 漏洞检查, 暂时不使用
