//
//  pip_icmp.cpp
//
//  Created by Plumk on 2022/1/13.
//

#include "pip_icmp.hpp"
#include "pip_netif.hpp"
#include "pip_debug.hpp"


void pip_icmp::input(const void *bytes, pip_ip_header *ip_data) {
    
    pip_uint16 datalen = ip_data->datalen - ip_data->headerlen;
    
#if PIP_DEBUG
    struct icmp *hdr = (struct icmp *)bytes;
    pip_debug_output_icmp(hdr, "icmp_input");
#endif
    
    pip_netif * netif = pip_netif::shared();
    if (netif->received_icmp_data_callback) {
        netif->received_icmp_data_callback(netif, (void *)bytes, datalen, ip_data->src_str, ip_data->dst_str, ip_data->ttl);
    }
    
    delete ip_data;
}


void pip_icmp::output(const void *buffer, pip_uint16 buffer_len, const char * src_ip, const char * dst_ip) {
    
    pip_buf * payload_buf = new pip_buf((void *)buffer, buffer_len, 0);
    
    pip_in_addr src = { inet_addr(src_ip) };
    pip_in_addr dst = { inet_addr(dst_ip) };
    
    pip_netif::shared()->output(payload_buf, IPPROTO_ICMP, src, dst);
    delete payload_buf;
    
}
