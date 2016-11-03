#include "./def/dev_def.h"
#include "util/dev_udp.h"
#include "util/dev_if_so.h"
#include "util/dev_utils.h"

#include "dev_protocol.h"
#include "dev_board.h"


static char protocol_buf[2048] = {0};
static int portNum = 88;
#define MSG_HEAD_LEN sizeof(msg_head_t)

int 
dev_protocol_port(void) 
{
    return portNum;
}

static inline void
dev_msg_head(msg_head_t *msg, uint8_t type, uint16_t len)
{
    msg->version = 0x02;
    msg->type = type;
    if (len) msg->len = htons(len);
}
/*
    uint8_t   slot;
    uint8_t   slot_type;
    uint16_t  board_type;
    uint32_t  seq;
    uint32_t  uptime;
    uint32_t  flag;
*/
int 
dev_master_probe(int slot, int slot_type, int seq, int flag)
{
    msg_head_t *msg = (msg_head_t *)protocol_buf;
    msg_probe_t *probe = (msg_probe_t *)msg->data;
    long uptime = dev_sys_uptime();

    probe->slot = (uint8_t)slot;
    probe->slot_type = (uint8_t)slot_type;
    probe->seq = htonl((uint32_t)seq);
    probe->uptime = htonll((uint64_t)uptime);
    probe->flag = htonl((uint32_t)flag);
    dev_msg_head(msg, DEV_RPROBE, sizeof(msg_probe_t));
    return (sizeof(msg_probe_t) + MSG_HEAD_LEN);
}

/*
int 
dev_register(int slot, int seq, int time, int boardstate, int boardtype, char *hw_ver, char *sw_ver)
{
    msg_head_t *msg = (msg_head_t *)protocol_buf;
    msg_register_t *reg = (msg_register_t *)msg->data;

    reg->seq = (uint32_t)seq;
    reg->slot = (uint8_t)slot;
    reg->time = htonl((uint32_t)time);
    reg->boardState = htonl((uint32_t)boardstate);
    reg->boardType = htonl((uint32_t)boardtype);
    snprintf((char *)reg->hwVersion, sizeof(reg->hwVersion), "%s", hw_ver);
    snprintf((char *)reg->swVersion, sizeof(reg->hwVersion), "%s", sw_ver);
    dev_msg_head(msg, DEV_REGISTER, sizeof(msg_register_t));

    return (sizeof(msg_register_t) + MSG_HEAD_LEN);
}

int 
dev_register_ack(int seq, int session_id)
{
    msg_head_t *msg = (msg_head_t *)protocol_buf;
    msg_register_ack_t *reg_ack = (msg_register_ack_t *)msg->data;

    reg_ack->seq = htonl((uint32_t)seq);
    reg_ack->sessionID = htonl((uint32_t)session_id);
    dev_msg_head(msg, DEV_REGISTER, sizeof(msg_register_ack_t));

    return (sizeof(msg_register_ack_t) + MSG_HEAD_LEN);
}

int 
dev_heart_beat(int seq, int session_id)
{
    msg_head_t *msg = (msg_head_t *)protocol_buf;
    msg_register_ack_t *reg_ack = (msg_register_ack_t *)msg->data;

    reg_ack->seq = htonl((uint32_t)seq);
    reg_ack->sessionID = htonl((uint32_t)session_id);
    dev_msg_head(msg, DEV_HEARTBEAT, sizeof(msg_register_ack_t));

    return (sizeof(msg_register_ack_t) + MSG_HEAD_LEN);
}
*/

/*int dev_master_exist_probe(void)
{
    msg_head_t *msg = (msg_head_t *)protocol_buf;
    dev_msg_head(msg, DEV_MASTER_EXISTS, 0);
    return (MSG_HEAD_LEN);
}*/
int
dev_sent_msg(int fd, int slotid, int msg_len)
{
    int i = 0;
    dev_udp_send_to_id(fd, slotid, portNum, protocol_buf, msg_len);
    return 0;
}


int
dev_sent_msg_array(int fd, slotid_array_t sid_array, int len)
{
    int i = 0;

    while (sid_array[i]) {
       dev_udp_send_to_id(fd, sid_array[i], portNum, protocol_buf, len);
       i++;
    }

    return 0;
}

int
dev_reply_msg(int fd, slotid_array_t sid_array, int len)
{
    int i = 0;

    while (sid_array[i]) {
       dev_udp_send_to_id(fd, sid_array[i], portNum, protocol_buf, len);
       i++;
    }

    return 0;
}




