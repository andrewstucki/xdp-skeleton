# XDP Skeleton

## Dependencies

**These instructions are for a Mac**. You'll need a recent LLVM installation that can target bpfeb and bpfel. By default the `Makefile` attempts to use an LLVM installation installed by Homebrew. If you don't have an installation at `/usr/local/opt/llvm/bin` you can run:

```bash
brew install llvm
```

Aside from LLVM, you'll need at least go 1.17 and your PATH variable including the default location that `go install` writes to.

After installing go, install Cilium's bpf2go utility:

```bash
go install github.com/cilium/ebpf/cmd/bpf2go@latest
```

## Building

Once all dependencies are installed, run `make`. You can test the output binary on any of the Vagrant machines by running `sudo ./xdp-skeleton -config config.hcl` in the `/vagrant` directory.