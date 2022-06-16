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
#include <os_event.h>

#define LIST_TASK_PRIO 5
#define LIST_STK_SIZE 1024

os_event_t* event = OS_NULL;

/* task_key */
static void Funtask_key(void *parameter)
{
		os_ubase_t count = 0;
		os_uint32_t recved_set = 0;
		while(1)
		{
			os_event_recv(event,0x01|0x02|0x04,OS_EVENT_OPTION_OR|OS_EVENT_OPTION_CLEAR,OS_WAIT_FOREVER,&recved_set);
			
			
			
			/*switch(rbuf.name)
			{
				case 'A':
				{
					os_kprintf("key0,lcd : RED\r\n");
					lcd_clear(WHITE);
					lcd_fill(10,100,100,230,RED);
					break;
				}
				case 'B':
				{
					os_kprintf("key1,lcd : GREEN\r\n");
					lcd_clear(WHITE);
					lcd_fill(10,100,100,230,GREEN);
					break;
				}
				case 'C':
				{
					os_kprintf("key2,lcd : BLUE\r\n");
					lcd_clear(WHITE);
					lcd_fill(10,100,100,230,BLUE);
					break;
				}
				default:
					break;
			}*/
			os_task_msleep(500);
		}
}

/* task_user */
static void Funtask_user(void *parameter)
{
		while(1)
		{
			if(!os_pin_read(key_table[0].pin))
			{
				os_kprintf("key0\r\n");
				os_event_send(event,0x01);
			}
			if(os_pin_read(key_table[1].pin))
			{
				os_kprintf("key1\r\n");
				os_event_send(event,0x02);
			}
			if(!os_pin_read(key_table[2].pin))
			{
				os_kprintf("key2\r\n");
				os_event_send(event,0x04);
			}
			os_task_msleep(200);
		}
}

int main(void)
{
		os_task_t* task_key = os_task_create("task_key", Funtask_key, NULL, LIST_STK_SIZE, LIST_TASK_PRIO);
		OS_ASSERT(task_key);
		os_task_startup(task_key);
	
		os_task_t* task_user = os_task_create("task_user", Funtask_user, NULL, LIST_STK_SIZE, LIST_TASK_PRIO);
		OS_ASSERT(task_user);
		os_task_startup(task_user);
	
		event = os_event_create("event");	
		
    return 0;
}