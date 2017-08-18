/*******************************************************************************
 *
 * Copyright (c) 2013, 2014, 2015 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#ifndef LWM2M_MEMORY_TRACE

void *lwm2m_malloc(size_t s)
{
	void *ptr = malloc(s);
	if(ptr != NULL)
	{
		// printf("===>  [%d] 0x%x = %s(%ld)\n", __LINE__, ptr, __func__, s);
		return ptr;		
	}
	else
	{
		printf("===>  %s failed. \n", __func__);
	}
}

void lwm2m_free(void *p)
{
	// printf("===>  [%d] %s(0x%x)\n", __LINE__, __func__, p);
	free(p);
}

char *lwm2m_strdup(const char *str)
{
	return strdup(str);
}

#endif

int lwm2m_strncmp(const char *s1,
				  const char *s2,
				  size_t n)
{
	return strncmp(s1, s2, n);
}

time_t lwm2m_gettime(void)
{
	struct timeval tv;

	if (0 != gettimeofday(&tv, NULL)) {
		return -1;
	}

	return tv.tv_sec;
}

void lwm2m_printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);

	vfprintf(stderr, format, ap);

	va_end(ap);
}
