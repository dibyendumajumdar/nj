/*******************************************************************************
 * Copyright (c) 2015, 2015 IBM Corp. and others
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

#ifndef HOOKGEN_HPP_
#define HOOKGEN_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "pugixml.hpp"

typedef enum RCType {
	RC_OK = 0,
	RC_FAILED = -1,
	RCType_EnsureWideEnum = 0x1000000 /* force 4-byte enum */
} RCType;

RCType startHookGen(int argc, char *argv[]);

class HookGen
{
	/*
	 * Data members
	 */
private:
	int _eventNum;
	const char *_fileName; /**< value from argv so no need to free */
	const char *_publicFileName; /**< value (or substring) from XML parsing so no need to free */
	FILE *_publicFile; /**< FILE handle for public header file.  close in tearDown if non-NULL */
	const char *_privateFileName; /**< value (or substring) from XML parsing so no need to free */
	FILE *_privateFile; /**< FILE handle for prviate header file.  close in tearDown if non-NULL */
	bool _verbose; /**< If we want to print verbose information to stderr */
protected:
public:

private:
	static char
	convertChar(char value)
	{
		if (('a' <= value) && (value <= 'z')) {
			return 'A' + value - 'a';
		} else if (value == '.') {
			return '_';
		}
		return value;
	}

	static char *getHeaderGateMacro(const char *fileName);
	static void writeComment(FILE *file, char *text);
	RCType startPublicHeader(const char *declarations);
	RCType completePublicHeader();
	RCType startPrivateHeader();
	RCType completePrivateHeader(const char *structName);
	void writeEventToPublicHeader(const char *name, const char *description, const char *condition, const char *structName, const char *reverse, pugi::xml_node event);
	void writeEventToPrivateHeader(const char *name, const char *condition, const char *once, int sampling, const char *structName, pugi::xml_node event);
	void writeEvent(pugi::xml_node event);

	static void displayUsage();

	static char *getAbsolutePath(const char *fileName);
	static const char *getActualFileName(const char *fileName);
	static FILE *createFile(char *path, const char *fileName);
	RCType createFiles(const char *publicFileName, const char *privateFileName);

protected:
public:
	HookGen()
		: _eventNum(1)
		, _fileName(NULL)
		, _publicFileName(NULL)
		, _publicFile(NULL)
		, _privateFileName(NULL)
		, _privateFile(NULL)
		, _verbose(false)
	{
	}

	~HookGen()
	{
	}

	RCType parseOptions(int argc, char *argv[]);
	RCType processFile();
	void tearDown();

	const char *getFileName() const {return _fileName;}
	const char *getPublicFileName() const {return _publicFileName;}
	const char *getPrivateFileName() const {return _privateFileName;}
	bool isVerbose() const {return _verbose;}
};

#endif /* HOOKGEN_HPP_ */
