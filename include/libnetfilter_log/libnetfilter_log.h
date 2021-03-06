/* libnetfilter_log.h: Header file for the Netfilter Userspace Log library.
 *
 * (C) 2005 by Harald Welte <laforge@gnumonks.org>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

#ifndef __LIBNETFILTER_LOG_H
#define __LIBNETFILTER_LOG_H

#include <stdint.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <libnetfilter_log/linux_nfnetlink_log.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nflog_handle;
struct nflog_g_handle;
struct nflog_data;

extern int nflog_errno;

extern struct nfnl_handle *nflog_nfnlh(struct nflog_handle *h);
extern int nflog_fd(struct nflog_handle *h);

typedef int nflog_callback(struct nflog_g_handle *gh, struct nfgenmsg *nfmsg,
			    struct nflog_data *nfd, void *data);


extern struct nflog_handle *nflog_open(void);
extern struct nflog_handle *nflog_open_nfnl(struct nfnl_handle *nfnlh);
extern int nflog_close(struct nflog_handle *h);

extern int nflog_bind_pf(struct nflog_handle *h, uint16_t pf);
extern int nflog_unbind_pf(struct nflog_handle *h, uint16_t pf);

extern struct nflog_g_handle *nflog_bind_group(struct nflog_handle *h,
						 uint16_t num);
extern int nflog_unbind_group(struct nflog_g_handle *gh);

extern int nflog_set_mode(struct nflog_g_handle *gh,
			  uint8_t mode, unsigned int len);
extern int nflog_set_timeout(struct nflog_g_handle *gh, uint32_t timeout);
extern int nflog_set_flags(struct nflog_g_handle *gh, uint16_t flags);
extern int nflog_set_qthresh(struct nflog_g_handle *gh, uint32_t qthresh);
extern int nflog_set_nlbufsiz(struct nflog_g_handle *gh, uint32_t nlbufsiz);

extern int nflog_callback_register(struct nflog_g_handle *gh,
				    nflog_callback *cb, void *data);
extern int nflog_handle_packet(struct nflog_handle *h, char *buf, int len);


extern struct nfulnl_msg_packet_hdr *nflog_get_msg_packet_hdr(struct nflog_data *nfad);

extern uint16_t nflog_get_hwtype(struct nflog_data *nfad);
extern uint16_t nflog_get_msg_packet_hwhdrlen(struct nflog_data *nfad);
extern char *nflog_get_msg_packet_hwhdr(struct nflog_data *nfad);

extern uint32_t nflog_get_nfmark(struct nflog_data *nfad);
extern int nflog_get_timestamp(struct nflog_data *nfad, struct timeval *tv);
extern uint32_t nflog_get_indev(struct nflog_data *nfad);
extern uint32_t nflog_get_physindev(struct nflog_data *nfad);
extern uint32_t nflog_get_outdev(struct nflog_data *nfad);
extern uint32_t nflog_get_physoutdev(struct nflog_data *nfad);
extern struct nfulnl_msg_packet_hw *nflog_get_packet_hw(struct nflog_data *nfad);
extern int nflog_get_payload(struct nflog_data *nfad, char **data);
extern char *nflog_get_prefix(struct nflog_data *nfad);
extern int nflog_get_uid(struct nflog_data *nfad, uint32_t *uid);
extern int nflog_get_gid(struct nflog_data *nfad, uint32_t *gid);
extern int nflog_get_seq(struct nflog_data *nfad, uint32_t *seq);
extern int nflog_get_seq_global(struct nflog_data *nfad, uint32_t *seq);

enum {
	NFLOG_XML_PREFIX	= (1 << 0),
	NFLOG_XML_HW		= (1 << 1),
	NFLOG_XML_MARK		= (1 << 2),
	NFLOG_XML_DEV		= (1 << 3),
	NFLOG_XML_PHYSDEV	= (1 << 4),
	NFLOG_XML_PAYLOAD	= (1 << 5),
	NFLOG_XML_TIME		= (1 << 6),
	NFLOG_XML_CTID		= (1 << 7),
	NFLOG_XML_ALL		= ~0U,
};

extern int nflog_snprintf_xml(char *buf, size_t len, struct nflog_data *tb, int flags);

extern struct nlmsghdr *
nflog_nlmsg_put_header(char *buf, uint8_t type, uint8_t family, uint16_t gnum);
extern int nflog_attr_put_cfg_mode(struct nlmsghdr *nlh, uint8_t mode, uint32_t range);
extern int nflog_attr_put_cfg_cmd(struct nlmsghdr *nlh, uint8_t cmd);
extern int nflog_nlmsg_parse(const struct nlmsghdr *nlh, struct nlattr **attr);

enum nflog_output_type {
	NFLOG_OUTPUT_XML	= 0,
};

int nflog_nlmsg_snprintf(char *buf, size_t bufsiz, const struct nlmsghdr *nlh,
			 struct nlattr **attr, enum nflog_output_type type,
			 uint32_t flags);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif	/* __LIBNETFILTER_LOG_H */
