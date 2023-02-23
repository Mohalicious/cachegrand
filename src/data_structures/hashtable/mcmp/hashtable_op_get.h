#ifndef CACHEGRAND_HASHTABLE_OP_GET_H
#define CACHEGRAND_HASHTABLE_OP_GET_H

#ifdef __cplusplus
extern "C" {
#endif

bool hashtable_mcmp_op_get(
        hashtable_t *hashtable,
        hashtable_key_data_t *key,
        hashtable_key_size_t key_size,
        hashtable_value_data_t *data);

#ifdef __cplusplus
}
#endif

#endif //CACHEGRAND_HASHTABLE_OP_GET_H
