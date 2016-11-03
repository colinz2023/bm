#include "core/dev_event_loop.h"
#include "core/dev_event.h"
#include "core/dev_event_timer.h"
#include "core/dev_signalfd.h"
#include "util/dev_pid.h"

#include "dev_board.h"
#include "dev_master.h"
#include "dev_io.h"

static int 
devd_tool(int argc, char *argv[])
{
    int opt;
    while( (opt = getopt(argc, argv, "i:p:scb")) != -1) {
        switch(opt) {
        
            break;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{   
    int res = 0;
    char *env_ptr = NULL;
    dev_event_t *ev_srv = NULL;
    dev_routine_t *rt = NULL;

    res = dev_daemon_init("dev2d");
    if (res > 0) {
        return devd_tool(argc, argv);
    }

    if (dev_event_deafult_loop_init(100) == NULL) {
        fprintf(stderr, "Fail to create deafult loop\n");
        exit(-1);
    }
 
    rt = malloc(sizeof(dev_routine_t));
    if (rt == NULL) {
        exit(-1);
    }
    rt->timer = dev_event_timer_creat(50, rt);
    dev_get_self_info(&rt->board_info);

    if (rt->board_info.slot_type == DEV_STATE_IO) {
        ev_srv = dev_io_creat(rt);
    } else {
        ev_srv = dev_master_creat(rt);
    }

    if (ev_srv == NULL) {
        fprintf(stderr, "%s\n", "ev_srv is NULL");
        exit(-1);
    }

    dev_event_loop_add(dev_event_deafult_loop(), dev_defualt_signalfd(NULL));
    dev_event_loop_add(dev_event_deafult_loop(), rt->timer);
    dev_event_loop_add(dev_event_deafult_loop(), ev_srv);
    dev_event_loop_run(dev_event_deafult_loop());
    
    return 0;
}
