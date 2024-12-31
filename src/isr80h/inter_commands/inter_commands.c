#include "inter_commands.h"
#include "task/task.h"
#include "string/string.h"
#include "fs/file.h"
#include "fs/pparser.h"
#include "fs/fat/fat16.h"
#include "disk/disk.h"
#include "kernel.h"

void* isr80h_command10_echo(struct interrupt_frame* frame)
{
    void* user_space_msg_buffer = task_get_stack_item(task_current(), 0);
    char buf[1024];
    copy_string_from_task(task_current(), user_space_msg_buffer, buf, sizeof(buf));

    print(buf);
    return 0;
}

void* isr80h_command11_ls(struct interrupt_frame* frame)
{
    void* path_from_user = task_get_stack_item(task_current(), 0);
    char buf[1024];
    copy_string_from_task(task_current(), path_from_user, buf, sizeof(buf));

    fat16_read_dir(buf);
    return 0;
}

void* isr80h_command12_mkdir(struct interrupt_frame* frame)
{
    void* path_from_user = task_get_stack_item(task_current(), 0);
    char buf[1024];
    copy_string_from_task(task_current(), path_from_user, buf, sizeof(buf));

    fat16_mkdir(buf);
    return 0;
}

void* isr80h_command13_clear(struct interrupt_frame* frame)
{
    terminal_clear();    
    return 0;
}
