.PHONY: fmt lint

build:
	echo build

# check if the tools are installed
GOBIN := $(shell go env GOPATH)/bin
GOFUMPT := $(GOBIN)/gofumpt
GOIMPORTS := $(GOBIN)/goimports
GOLINT := $(GOBIN)/golangci-lint

$(GOFUMPT):
	go install mvdan.cc/gofumpt@v0.5.0  # for go1.22

$(GOIMPORTS):
	go install golang.org/x/tools/cmd/goimports@v0.15.0  # for go1.22

$(GOLINT):
	go install github.com/golangci/golangci-lint/cmd/golangci-lint@v1.57.1  # for go1.22

fmt: $(GOFUMPT) $(GOIMPORTS)
	$(GOFUMPT) -w .
	$(GOIMPORTS) -w .

lint: $(GOLINT)
	- $(GOLINT) run .
