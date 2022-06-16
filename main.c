/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with 
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        main.c
 *
 * @brief       User application entry
 *
 * @revision
 * Date         Author          Notes
 * 2020-11-20   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#include <board.h>
#include <os_task.h>
#include <st7789vw.h>
#include <os_list.h>
#include <pin.h>
#include <os_mq.h>
#include <stdlib.h>

#define LIST_TASK_PRIO 4
#define LIST_STK_SIZE 2048

#define R 10

os_sem_t *sem = OS_NULL;
os_mq_t* mq = OS_NULL;
typedef struct key_data
{
  char message;
}key_data_t;

/* 节点坐标信息 */
struct node{
	os_uint16_t x;
	os_uint16_t y;
};
struct node snake_info[15];    /* 蛇节点最多15个 */
static os_uint16_t count = 0;  /* 长度 */

struct node* snode = OS_NULL;

os_uint16_t xnum[9] = {40,140,100,80,60,180,200,120,160};
os_uint16_t ynum[9] = {160,120,200,180,60,80,100,140,40};

void create_node(struct node* node)
{
	srand(time(NULL));
	int i = rand()%8;
	int j = rand()%8;
	node->x = xnum[i];
	node->y = ynum[j];
}

void left()
{
	os_kprintf("left\r\n");
	if(os_sem_wait(sem,OS_WAIT_FOREVER)==OS_EOK)
	{
		/* 计算所有节点 */
		struct node pre_node = snake_info[0];
		for(os_uint16_t i = 0;i<count;i++)
		{
			if(i==0)/* 头 */
			{
				if(snake_info[i].x - (R*2) >= 0)
				{
					snake_info[i].x = snake_info[i].x - (R*2);
					if(snake_info[i].x == snode->x && snake_info[i].y == snode->y)/* 吃到点 */
					{
						os_kprintf("eat food\r\n");
						free(snode);
						snode = OS_NULL;
						count++;
					}
				}
				else
				{
					os_kprintf("game over\r\n");
					exit(0);
				}
			}
			else
			{
				/* 身体 */
				struct node temp_node = snake_info[i];
				snake_info[i].x = pre_node.x;
				snake_info[i].y = pre_node.y;
				pre_node = temp_node;
			}
		}
	}
	os_sem_post(sem);
	/* 绘制所有节点 */
	lcd_clear(YELLOW);
	lcd_set_color(YELLOW,RED);
	lcd_draw_rectangle(0,0,240,240);
	lcd_set_color(BLUE,BLUE);
	for(os_uint16_t i=0;i<count;i++)
	{
		lcd_draw_circle(snake_info[i].x,snake_info[i].y,R);
		os_kprintf("x:%d y:%d\r\n",snake_info[i].x,snake_info[i].y);
	}
	
	/* 随机节点 */
	lcd_set_color(GRAY,GRAY);
	if(snode == OS_NULL)
	{
		snode = (struct node*)os_malloc(sizeof(struct node));
		create_node(snode);
		lcd_draw_circle(snode->x,snode->y,R);
	}
	else
		lcd_draw_circle(snode->x,snode->y,R);
}
void right()
{
	os_kprintf("right\r\n");
	/* 计算所有节点 */
	struct node pre_node = snake_info[0];
	for(os_uint16_t i = 0;i<count;i++)
	{
		if(i==0)/* 头 */
		{
			if(snake_info[i].x + (R*2) <= 240)
			{
				snake_info[i].x = snake_info[i].x + (R*2);
				if(snake_info[i].x == snode->x && snake_info[i].y == snode->y)/* 吃到点 */
				{
					os_kprintf("eat food\r\n");
					free(snode);
					snode = OS_NULL;
					count++;
				}
			}
			else
			{
				os_kprintf("game over\r\n");
				exit(0);
			}
		}
		else
		{
			/* 身体 */
			struct node temp_node = snake_info[i];
			snake_info[i].x = pre_node.x;
			snake_info[i].y = pre_node.y;
			pre_node = temp_node;
		}
	}
	/* 绘制所有节点 */
	lcd_clear(YELLOW);
	lcd_set_color(YELLOW,RED);
	lcd_draw_rectangle(0,0,240,240);
	lcd_set_color(BLUE,BLUE);
	for(os_uint16_t i=0;i<count;i++)
	{
		lcd_draw_circle(snake_info[i].x,snake_info[i].y,R);
		os_kprintf("x:%d y:%d\r\n",snake_info[i].x,snake_info[i].y);
	}
	/* 随机节点 */
	lcd_set_color(GRAY,GRAY);
	if(snode == OS_NULL)
	{
		snode = (struct node*)os_malloc(sizeof(struct node));
		create_node(snode);
		lcd_draw_circle(snode->x,snode->y,R);
	}
	else
		lcd_draw_circle(snode->x,snode->y,R);
}
void up()
{
	os_kprintf("up\r\n");
	/* 计算所有节点 */
	struct node pre_node = snake_info[0];
	for(os_uint16_t i = 0;i<count;i++)
	{
		if(i==0)/* 头 */
		{
			if(snake_info[i].y - (R*2) >= 0)
			{
				snake_info[i].y = snake_info[i].y - (R*2);
				if(snake_info[i].x == snode->x && snake_info[i].y == snode->y)/* 吃到点 */
				{
					os_kprintf("eat food\r\n");
					free(snode);
					snode = OS_NULL;
					count++;
				}
			}
			else
			{
				os_kprintf("game over\r\n");
				exit(0);
			}
		}
		else
		{
			/* 身体 */
			struct node temp_node = snake_info[i];
			snake_info[i].x = pre_node.x;
			snake_info[i].y = pre_node.y;
			pre_node = temp_node;
		}
	}
	/* 绘制所有节点 */
	lcd_clear(YELLOW);
	lcd_set_color(YELLOW,RED);
	lcd_draw_rectangle(0,0,240,240);
	lcd_set_color(BLUE,BLUE);
	for(os_uint16_t i=0;i<count;i++)
	{
		lcd_draw_circle(snake_info[i].x,snake_info[i].y,R);
		os_kprintf("x:%d y:%d\r\n",snake_info[i].x,snake_info[i].y);
	}
	/* 随机节点 */
	lcd_set_color(GRAY,GRAY);
	if(snode == OS_NULL)
	{
		snode = (struct node*)os_malloc(sizeof(struct node));
		create_node(snode);
		lcd_draw_circle(snode->x,snode->y,R);
	}
	else
		lcd_draw_circle(snode->x,snode->y,R);
}
void down()
{
	os_kprintf("down\r\n");
	/* 计算所有节点 */
	struct node pre_node = snake_info[0];
	for(os_uint16_t i = 0;i<count;i++)
	{
		if(i==0)/* 头 */
		{
			if(snake_info[i].y + (R*2) <= 240)
			{
				snake_info[i].y = snake_info[i].y + (R*2);
				if(snake_info[i].x == snode->x && snake_info[i].y == snode->y)/* 吃到点 */
				{
					os_kprintf("eat food\r\n");
					free(snode);
					snode = OS_NULL;
					count++;
				}
			}
			else
			{
				os_kprintf("game over\r\n");
				exit(0);
			}
		}
		else
		{
			/* 身体 */
			struct node temp_node = snake_info[i];
			snake_info[i].x = pre_node.x;
			snake_info[i].y = pre_node.y;
			pre_node = temp_node;
		}
	}
	/* 绘制所有节点 */
	lcd_clear(YELLOW);
	lcd_set_color(YELLOW,RED);
	lcd_draw_rectangle(0,0,240,240);
	lcd_set_color(BLUE,BLUE);
	for(os_uint16_t i=0;i<count;i++)
	{
		lcd_draw_circle(snake_info[i].x,snake_info[i].y,R);
		os_kprintf("x:%d y:%d\r\n",snake_info[i].x,snake_info[i].y);
	}
	/* 随机节点 */
	lcd_set_color(GRAY,GRAY);
	if(snode == OS_NULL)
	{
		snode = (struct node*)os_malloc(sizeof(struct node));
		create_node(snode);
		lcd_draw_circle(snode->x,snode->y,R);
	}
	else
		lcd_draw_circle(snode->x,snode->y,R);
}

/* 刷新显示 */
static void show_taskfun(void *parameter)
{
	/* 初始化 */
	lcd_clear(YELLOW);
	lcd_set_color(YELLOW,RED);
	lcd_draw_rectangle(0,0,240,240);
	lcd_set_color(BLUE,BLUE);
	lcd_draw_circle(240/2,240/2,R);
	snake_info[0].x = 240/2;
	snake_info[0].y = 240/2;
	count = 1;
	os_task_msleep(3000);
	int flag = 0;
	os_kprintf("Game start\r\n");

	struct key_data buf;
	os_size_t recv_size;
	/* 初始化起点和随机点 */
	if(snode == OS_NULL)
	{
		snode = (struct node*)os_malloc(sizeof(struct node));
		create_node(snode);
		lcd_clear(YELLOW);
		lcd_set_color(YELLOW,RED);
		lcd_draw_rectangle(0,0,240,240);
		lcd_set_color(BLUE,BLUE);
		for(os_uint16_t i = 0;i<count;i++)
			lcd_draw_circle(snake_info[i].x,snake_info[i].y,R);
		lcd_set_color(GRAY,GRAY);
		lcd_draw_circle(snode->x,snode->y,R);
	}
	while(1)
	{
		/* 按键转向移动 */
		if(OS_EOK == os_mq_recv(mq,&buf, sizeof(struct key_data), OS_NO_WAIT, &recv_size)){
			if(buf.message=='A')
				left();
			else if(buf.message=='B')
				right();
			else if(buf.message=='C')
				up();
			else
				down();
			flag = 1;
		}
		/* 自方向移动 */
		if(flag)
		{
			if(buf.message=='A')
				left();
			else if(buf.message=='B')
				right();
			else if(buf.message=='C')
				up();
			else
				down();
		}
		os_task_msleep(500);
	}
}

/* 按键任务 */
static void key_taskfun(void *parameter)
{
	struct key_data a,b,c,d;
	a.message='A';
	b.message='B';
	c.message='C';
	d.message='D';
	while(1)
	{
		if(!os_pin_read(key_table[0].pin))
		{
			os_mq_send(mq,&a, sizeof(struct key_data),OS_WAIT_FOREVER);
		}
		if(os_pin_read(key_table[1].pin))
		{
			os_mq_send(mq,&d, sizeof(struct key_data),OS_WAIT_FOREVER);
		}
		if(!os_pin_read(key_table[2].pin))
		{
			os_mq_send(mq,&b, sizeof(struct key_data),OS_WAIT_FOREVER);
		}
		if(!os_pin_read(key_table[3].pin))
		{
			os_mq_send(mq,&c, sizeof(struct key_data),OS_WAIT_FOREVER);
		}
		os_task_msleep(500);
	}
}

int main(void)
{
	sem = os_sem_create("sem",1,3);
	mq = os_mq_create("mq",10,5);
	os_task_t* key_task = os_task_create("key_task", key_taskfun, NULL, LIST_STK_SIZE, LIST_TASK_PRIO);
	os_task_t* show_task = os_task_create("show_task", show_taskfun, NULL, LIST_STK_SIZE, LIST_TASK_PRIO);
	OS_ASSERT(key_task);
	OS_ASSERT(show_task);
	os_task_startup(show_task);
	os_task_startup(key_task);
	
  return 0;
}
