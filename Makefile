LLVM_PATH ?= /usr/local/opt/llvm/bin

CLANG ?= $(LLVM_PATH)/clang
STRIP ?= $(LLVM_PATH)/llvm-strip
CFLAGS := -O2 -g -Wall -Werror $(CFLAGS)
GOOS := linux
GOLDFLAGS := -s -w

xdp/bpf_bpfel.go: export BPF_STRIP := $(STRIP)
xdp/bpf_bpfel.go: export BPF_CLANG := $(CLANG)
xdp/bpf_bpfel.go: export BPF_CFLAGS := $(CFLAGS)
xdp/bpf_bpfel.go: xdp/xdp.c
	go generate ./...

.PHONY: generate
generate: xdp/bpf_bpfel.go

xdp-skeleton: export GOOS := $(GOOS)
xdp-skeleton: generate
	go build -ldflags "$(GOLDFLAGS)"

clean:
	@rm -f xdp/bpf_* xdp-skeleton

.DEFAULT_GOAL := xdp-skeleton