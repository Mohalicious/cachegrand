/**
 * Copyright (C) 2018-2022 Daniele Salvatore Albano
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 **/

#include <catch2/catch.hpp>
#include <cstring>
#include <arpa/inet.h>

#include "../../enum-flags-operators.hpp"

#include "misc.h"
#include "exttypes.h"
#include "config.h"
#include "clock.h"
#include "spinlock.h"
#include "transaction.h"
#include "transaction_spinlock.h"
#include "data_structures/hashtable/mcmp/hashtable.h"
#include "data_structures/double_linked_list/double_linked_list.h"
#include "data_structures/ring_bounded_queue_spsc/ring_bounded_queue_spsc_voidptr.h"
#include "data_structures/queue_mpmc/queue_mpmc.h"
#include "data_structures/hashtable/spsc/hashtable_spsc.h"
#include "memory_allocator/ffma.h"
#include "module/module.h"
#include "network/io/network_io_common.h"
#include "network/channel/network_channel.h"
#include "storage/io/storage_io_common.h"
#include "storage/channel/storage_channel.h"
#include "storage/db/storage_db.h"
#include "protocol/redis/protocol_redis.h"
#include "protocol/redis/protocol_redis_reader.h"
#include "module/redis/module_redis.h"
#include "module/redis/module_redis_connection.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wpointer-arith"

#include "module/redis/module_redis_command.h"

#include "module_redis_autogenerated_commands_contexts.h"
#include "module_redis_autogenerated_commands_enum.h"

extern module_redis_command_argument_t module_redis_command_sort_arguments[];
extern module_redis_command_argument_t module_redis_command_get_arguments[];

TEST_CASE("module/redis/module_redis_command.c", "[module][redis][module_redis_command]") {
    module_redis_command_info_t command_infos_map[] = {
            {
                    .string = { 'S', 'O', 'R', 'T', 0 },
                    .string_len = (uint8_t)strlen("SORT"),
                    .command = MODULE_REDIS_COMMAND_SORT,
                    .context_size = sizeof(module_redis_command_sort_context_t),
                    .arguments_count = 7,
                    .required_arguments_count = 3,
                    .arguments = module_redis_command_sort_arguments,
                    .tokens_hashtable = nullptr,
            },
            {
                    .string = { 'G', 'E', 'T', 0 },
                    .string_len = (uint8_t)strlen("GET"),
                    .command = MODULE_REDIS_COMMAND_GET,
                    .context_size = sizeof(module_redis_command_get_context_t),
                    .arguments_count = 1,
                    .required_arguments_count = 1,
                    .arguments = module_redis_command_get_arguments,
                    .tokens_hashtable = nullptr,
            },
    };

    SECTION("Ensure that auto generated struct and data haven't changed") {
        SECTION("sizeof(module_redis_command_sort_context_t)") {
            REQUIRE(sizeof(module_redis_command_sort_context_t) == 120);
        }

        SECTION("sizeof(module_redis_command_sort_context_subargument_order_t)") {
            REQUIRE(sizeof(module_redis_command_sort_context_subargument_order_t) == 2);
        }

        SECTION("sizeof(module_redis_command_sort_context_subargument_limit_offset_count_t)") {
            REQUIRE(sizeof(module_redis_command_sort_context_subargument_limit_offset_count_t) == 16);
        }
    }

//    SECTION("module_redis_command_process_begin") {
//        module_redis_command_context_t *command_context = module_redis_command_process_begin(
//                &test_module_redis_command_sort_command_info);
//
//        ffma_mem_free(command_context);
//    }
}