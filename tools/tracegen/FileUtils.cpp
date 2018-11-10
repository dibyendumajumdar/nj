/*******************************************************************************
 * Copyright (c) 2014, 2016 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/
#if defined(J9ZOS390)
/* needed to expose snprintf() */
#define _ISOC99_SOURCE
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "FileUtils.hpp"

const char *
FileUtils::getFileName(const char *path)
{
	const char *filename = strrchr(path, PATH_SEP[0]);
	if (filename == NULL) {
		filename = path;
	} else {
		filename++;
	}
	return filename;
}

const char *
FileUtils::getFileExt(const char *fileName)
{
	const char *dot = strrchr(fileName, '.');
	/* Ignore foo and starting with .foo */
	if (!dot || dot == fileName) {
		return NULL;
	} else {
		return dot + 1;
	}
}

const char *
FileUtils::getTargetFileName(J9TDFOptions *options, const char *tdfFileName, const char *filePrefix, const char *fileName, const char *extension)
{
	char *baseName = NULL;
	char *filePath = NULL;
	size_t filePathLen = 0;
	if (options->writeToCurrentDir || (NULL == strpbrk(tdfFileName, "/\\"))) {
		if (NULL == (baseName = Port::omrfile_getcwd())) {
			eprintf("Failed to get current directory");
			goto failed;
		}
	} else {
		baseName = (char *)Port::omrmem_calloc(1, strlen(tdfFileName) + 1);
		if (NULL == baseName) {
			eprintf("Failed to allocate memory");
			goto failed;
		}
		strcpy(baseName, tdfFileName);
		char *end = baseName + strlen(tdfFileName);
		/* Trim baseName down to just the directory name,
		 * we can use this as the basis for the file names for the
		 * output files.
		 */
		while (end > baseName && *end != '/' && *end != '\\') {
			*end = '\0';
			end--;
		}
	}

	filePathLen = strlen(baseName) + 1 + strlen(filePrefix) + strlen(fileName) + strlen(extension) + 1;
	filePath = (char *)Port::omrmem_calloc(1, filePathLen);
	if (NULL == filePath) {
		eprintf("Failed to allocate memory");
		goto failed;
	}

	if (baseName[strlen(baseName) - 1] == PATH_SEP[0]) {
		snprintf(filePath, filePathLen, "%s%s%s%s", baseName, filePrefix, fileName, extension);
	} else {
		snprintf(filePath, filePathLen, "%s" PATH_SEP "%s%s%s", baseName, filePrefix, fileName, extension);
	}

	Port::omrmem_free((void **)&baseName);
	return filePath;

failed:
	Port::omrmem_free((void **)&baseName);
	Port::omrmem_free((void **)&filePath);
	return NULL;
}

time_t
FileUtils::getMtime(const char *filePath)
{
	dirstat statbuf;
	if (RC_FAILED == Port::stat(filePath, &statbuf)) {
		return 0;
	}
	return statbuf.st_mtime;
}

RCType
FileUtils::visitDirectory(J9TDFOptions *options, const char *dirName, const char *fileExtFilter, void *targetObject, VisitFileCallBack callback)
{
	char *resultBuffer = NULL;
	RCType rc = RC_OK;
	intptr_t handle = PORT_INVALID_FIND_FILE_HANDLE;

	rc = Port::omrfile_findfirst(dirName, &resultBuffer, &handle);
	if (RC_OK != rc) {
		printError("Failed to open directory %s\n", dirName);
		goto failed;
	}

	while (RC_FAILED != rc) {
		char *nextEntry = NULL;
		/* skip current and parent dir */
		if (resultBuffer[0] == '.' || 0 == strcmp(resultBuffer, "..")) {
			Port::omrmem_free((void **)&resultBuffer);
			rc = Port::omrfile_findnext(handle, &resultBuffer);
			continue;
		}

		size_t nextEntrySize = strlen(dirName) + 1 /* PATH_SEP */ + strlen(resultBuffer) + 1;
		nextEntry = (char *)Port::omrmem_calloc(1, nextEntrySize);
		if (NULL == nextEntry) {
			goto failed;
		}
		snprintf(nextEntry, nextEntrySize, "%s" PATH_SEP "%s", dirName, resultBuffer);

		struct J9FileStat buf;
		if (RC_OK != Port::omrfile_stat(nextEntry, 0, &buf)) {
			printError("stat failed %s\n", nextEntry);
			goto failed;
		}

		if (buf.isDir) {
			visitDirectory(options, nextEntry, fileExtFilter, targetObject, callback);
		} else {
			const char *ext = FileUtils::getFileExt(resultBuffer);
			if (NULL != ext) {
				if (0 == strcmp(ext, fileExtFilter)) {
					callback(targetObject, options, nextEntry);
				}
			}
		}
		Port::omrmem_free((void **)&nextEntry);
		Port::omrmem_free((void **)&resultBuffer);
		rc = Port::omrfile_findnext(handle, &resultBuffer);
	}
	Port::omrfile_findclose(handle);

	return RC_OK;

failed:
	Port::omrfile_findclose(handle);
	Port::omrmem_free((void **)&resultBuffer);
	return RC_FAILED;
}

void
FileUtils::printError(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}
