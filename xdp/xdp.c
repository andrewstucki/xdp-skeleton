// +build ignore

#include "vmlinux.h"
#include "bpf_helpers.h"
#include "bpf_endian.h"

#define ETH_P_IP 0x0800

#define ensure_size(packet, value)                                             \
  ({                                                                           \
    if ((void *)((void *)packet->current + value) > (void *)packet->end)       \
      return -1;                                                               \
  })

struct packet_parser {
  void *current;
  void *end;
};

static __always_inline int parse_ethernet(struct packet_parser *packet, struct ethhdr **eth) {
  ensure_size(packet, sizeof(struct ethhdr));

  *eth = packet->current;
  packet->current += sizeof(struct ethhdr);
  return 0;
}

static __always_inline int parse_ip(struct packet_parser *packet, struct iphdr **ip) {
  ensure_size(packet, sizeof(struct iphdr));

  struct iphdr *header = (struct iphdr *)packet->current;
  u32 header_size = header->ihl << 2;
  if (header_size < sizeof(struct iphdr)) {
    return -1;
  }

  ensure_size(packet, header_size);
  packet->current += header_size;
  *ip = header;
  return 0;
}

static __always_inline int classify_ip(struct packet_parser *packet) {
  struct iphdr *ip;

  if (!parse_ip(packet, &ip)) {
    // TODO: fill in
    return XDP_PASS;
  }
  return XDP_PASS;
}

SEC("xdp_classifier")
int classifier(struct xdp_md *ctx) {
  struct packet_parser packet = {
      .current = (void *)(long)ctx->data,
      .end = (void *)(long)ctx->data_end,
  };

  struct ethhdr *eth;
  if (!parse_ethernet(&packet, &eth) && bpf_htons(ETH_P_IP) == eth->h_proto) {
    return classify_ip(&packet);
  }

  return XDP_PASS;
}

char __license[] SEC("license") = "BSD3";