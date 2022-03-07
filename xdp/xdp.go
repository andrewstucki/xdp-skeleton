package xdp

import (
	"context"
	"net"

	"github.com/cilium/ebpf/link"
)

//go:generate bpf2go -strip $BPF_STRIP -cc $BPF_CLANG -cflags $BPF_CFLAGS bpf xdp.c -- -I./headers

func Start(ctx context.Context, interfaces []net.Interface) error {
	objs := bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		return err
	}

	for _, iface := range interfaces {
		xdp, err := link.AttachXDP(link.XDPOptions{
			Program:   objs.Classifier,
			Interface: iface.Index,
		})
		if err != nil {
			return err
		}
		defer xdp.Close()
	}

	<-ctx.Done()
	return nil
}
