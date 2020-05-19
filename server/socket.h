/*
 * socket.h - Asynchronous DiME socket
 * Copyright (c) 2020 Nicholas West, Hantao Cui, CURENT, et. al.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * This software is provided "as is" and the author disclaims all
 * warranties with regard to this software including all implied warranties
 * of merchantability and fitness. In no event shall the author be liable
 * for any special, direct, indirect, or consequential damages or any
 * damages whatsoever resulting from loss of use, data or profits, whether
 * in an action of contract, negligence or other tortious action, arising
 * out of or in connection with the use or performance of this software.
 */

/**
 * @file socket.h
 * @brief Asynchronous DiME socket
 * @author Nicholas West
 * @date 2020
 */

#include <stddef.h>
#include <sys/types.h>

#include <jansson.h>
#include "ringbuffer.h"

#ifndef __DIME_socket_H
#define __DIME_socket_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int fd; /** File descriptor */

    dime_ringbuffer_t rbuf; /** Inbuffer */
    dime_ringbuffer_t wbuf; /** Outbuffer */
} dime_socket_t;

/**
 * @brief Initialize a new socket
 *
 * @param sock Pointer to a @c dime_socket_t struct
 * @param fd File descriptor to send/receive on
 *
 * @return A nonnegative value on success, or a negative value on failure
 */
int dime_socket_init(dime_socket_t *sock, int fd);

/**
 * @brief Free resources used by a socket
 *
 * @param sock Pointer to a @c dime_socket_t struct
 */
void dime_socket_destroy(dime_socket_t *sock);

/**
 * @brief Adds a DiME message to the outbuffer
 *
 * Composes a DiME message from the JSON data in @em jsondata and the binary data in @em bindata, and adds the message to the outbuffer. The message is not sent directly; subsequent calls to @link dime_socket_sendpartial @endlink will send the message at some point in the future.
 *
 * @param sock Pointer to a @c dime_socket_t struct
 * @param jsondata JSON portion of the message to send
 * @param bindata Binary portion of the message to send
 * @param bindata_len Length of binary data
 *
 * @return A nonnegative value on success, or a negative value on failure
 */
int dime_socket_sendfuture(dime_socket_t *sock, const json_t *jsondata, const void *bindata, size_t bindata_len);

/**
 * @brief Sends data in the outbuffer
 *
 * Sends any data written to the outbuffer by @link dime_socket_sendfuture @endlink. Does not attempt to send all of the data, so multiple calls may be necessary if that is desired.
 *
 * @param sock Pointer to a @c dime_socket_t struct
 *
 * @return Number of bytes sent on success, or a negative value on failure
 */
ssize_t dime_socket_sendpartial(dime_socket_t *sock);

/**
 * @brief Recieves data and writes it to the inbuffer
 *
 * Attempts to receive a given amount of data. If data is received, it is written to the inbuffer and the program attempts to construct a DiME message from the data in the inbuffer. If there is enough data to construct a message, @em jsondata, @em bindata, and @em bindata_len are updated with the corresponding data read from the message. If a message is received, @em jsondata and @em bindata should be freed with @c json_decref and @em free, respectively, once they are no longer needed.
 *
 * @param sock Pointer to a @c dime_socket_t struct
 * @param jsondata Pointer to the JSON portion of the message received
 * @param bindata Pointer to the binary portion of the message received
 * @param bindata_len Pointer to the length of the binary data
 *
 * @return Number of bytes read on a successful message decoding, a negative value on failure, zero otherwise
 */
ssize_t dime_socket_recvpartial(dime_socket_t *sock, json_t **jsondata, void **bindata, size_t *bindata_len);

/**
 * @brief Get the file descriptor of the socket
 *
 * @param sock Pointer to a @c dime_socket_t struct
 *
 * @return File descriptor
 */
int dime_socket_fd(const dime_socket_t *sock);

/**
 * @brief Get the number of bytes in the outbuffer of the socket
 *
 * @param sock Pointer to a @c dime_socket_t struct
 *
 * @return Number of bytes in the outbuffer
 */
size_t dime_socket_sendlen(const dime_socket_t *sock);

/**
 * @brief Get the number of bytes in the inbuffer of the socket
 *
 * @param sock Pointer to a @c dime_socket_t struct
 *
 * @return Number of bytes in the inbuffer
 */
size_t dime_socket_recvlen(const dime_socket_t *sock);

#ifdef __cplusplus
}
#endif

#endif
