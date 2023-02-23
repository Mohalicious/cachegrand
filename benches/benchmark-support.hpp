/**
 * Copyright (C) 2018-2022 Daniele Salvatore Albano
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 **/

#include <cstring>
#include <cmath>
#include <cstdint>

#include <numa.h>
#include <benchmark/benchmark.h>

#include "exttypes.h"
#include "spinlock.h"
#include "utils_cpu.h"
#include "misc.h"
#include "log/log.h"

#include "data_structures/hashtable/mcmp/hashtable.h"

#define BENCHES_MAX_THREADS_PER_CORE        32

const char* tag = "bench/support";

class BenchmarkSupport {
public:
    static bool CheckIfTooManyThreadsPerCore(int threads, int max_threads_per_core) {
        return (threads / utils_cpu_count()) > max_threads_per_core;
    }

    static void CollectHashtableStats(
            hashtable_t* hashtable,
            uint64_t* return_used_chunks,
            double* return_load_factor_chunks,
            uint64_t* return_used_max_overflowed_chunks_counter,
            double* return_used_avg_overflowed_chunks_counter,
            uint64_t* return_overflowed_chunks_count,
            hashtable_half_hashes_chunk_volatile_t** return_longest_half_hashes_chunk) {
        volatile hashtable_data_t* ht_data = hashtable->ht_current;

        uint64_t used_chunks = 0;
        double used_avg_overflowed_chunks_counter = 0;
        hashtable_chunk_count_t used_max_overflowed_chunks_counter = 0;
        *return_overflowed_chunks_count = 0;

        for(hashtable_chunk_index_t chunk_index = 0; chunk_index < ht_data->chunks_count; chunk_index++) {
            if (ht_data->half_hashes_chunk[chunk_index].half_hashes[0].slot_id == 0) {
                continue;
            }

            used_avg_overflowed_chunks_counter += ht_data->half_hashes_chunk[chunk_index].metadata.overflowed_chunks_counter;
            used_chunks++;

            if (ht_data->half_hashes_chunk[chunk_index].metadata.overflowed_chunks_counter > 0) {
                *return_overflowed_chunks_count++;
            }

            if (ht_data->half_hashes_chunk[chunk_index].metadata.overflowed_chunks_counter > used_max_overflowed_chunks_counter) {
                used_max_overflowed_chunks_counter = ht_data->half_hashes_chunk[chunk_index].metadata.overflowed_chunks_counter;
                *return_longest_half_hashes_chunk = &ht_data->half_hashes_chunk[chunk_index];
            }
        }

        *return_used_chunks = used_chunks;
        *return_used_max_overflowed_chunks_counter = used_max_overflowed_chunks_counter;
        *return_used_avg_overflowed_chunks_counter = (double)used_avg_overflowed_chunks_counter / (double)used_chunks;
        *return_load_factor_chunks = (double)used_chunks / (double)ht_data->chunks_count;
    }

    static void CollectHashtableStatsAndUpdateState(benchmark::State& state, hashtable_t* hashtable) {
        uint64_t used_chunks;
        double load_factor_chunks;
        double used_avg_overflowed_chunks_counter;
        uint64_t overflowed_chunks_count;
        uint64_t used_max_overflowed_chunks_counter;

        double requested_load_factor = (double) state.range(1) / 100.0f;
        uint64_t keys_count = (uint64_t) (((double) state.range(0)) * requested_load_factor);

        hashtable_half_hashes_chunk_volatile_t* longest_half_hashes_chunk;
        CollectHashtableStats(
                hashtable,
                &used_chunks,
                &load_factor_chunks,
                &overflowed_chunks_count,
                &used_avg_overflowed_chunks_counter,
                &used_max_overflowed_chunks_counter,
                &longest_half_hashes_chunk);

        state.counters["total_buckets"] = (double)hashtable->ht_current->buckets_count;
        state.counters["keys_count"] = keys_count;
        state.counters["requested_load_factor"] = requested_load_factor;
        state.counters["total_chunks"] = (double)hashtable->ht_current->chunks_count;
        state.counters["used_chunks"] = (double)used_chunks;
        state.counters["load_factor_chunks"] = load_factor_chunks;
        state.counters["overflowed_chunks_count"] = (double)overflowed_chunks_count;
        state.counters["used_avg_overflowed_chunks_counter"] = used_avg_overflowed_chunks_counter;
        state.counters["used_max_overflowed_chunks_counter"] = (double)used_max_overflowed_chunks_counter;

//    test_support_hashtable_print_heatmap(hashtable, 32);
    }
};
