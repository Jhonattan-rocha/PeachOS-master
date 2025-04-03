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

void* isr80h_command15_ls_v2(struct interrupt_frame* frame)
{
    void* user_path = task_get_stack_item(task_current(), 0);
    void* user_output = task_get_stack_item(task_current(), 1);
    int max_items = (int)(int)task_get_stack_item(task_current(), 2);

    char path[PEACHOS_MAX_PATH];
    copy_string_from_task(task_current(), user_path, path, sizeof(path));

    char temp[64][PEACHOS_MAX_PATH];
    int count = fat16_read_dir_return(path, temp, max_items);

    for (int i = 0; i < count; i++) {
        void* target = (char*)user_output + (i * PEACHOS_MAX_PATH);
        copy_to_task(task_current(), target, temp[i], PEACHOS_MAX_PATH);
    }

    return (void*)(int)count;
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

void* isr80h_command14_cat(struct interrupt_frame* frame){
    void* path_from_user = task_get_stack_item(task_current(), 0);
    char buf[1024];
    copy_string_from_task(task_current(), path_from_user, buf, sizeof(buf));

    fat16_cat(buf);
    return 0;
}
