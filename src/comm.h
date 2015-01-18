#include <pebble.h>

#define MAX_NAME_LEN 60

typedef struct {
	int t_id;
	char name[MAX_NAME_LEN];
	int nTarget;
	int nCompleted;
} task_t;

task_t* get_tasks();

task_t* get_task(int index);

int get_num_tasks();