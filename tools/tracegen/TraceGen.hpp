/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp. and others
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

#ifndef TRACEGEN_HPP_
#define TRACEGEN_HPP_

#include "FileReader.hpp"
#include "Port.hpp"
#include "TDFParser.hpp"
#include "TDFTypes.hpp"

RCType startTraceGen(int argc, char *argv[]);

class TraceGen
{
	/*
	 * Data members
	 */
private:
	Path *_visitedFile;
protected:
public:

private:
	/**
	 * Allocate new group and assign groupName.
	 * @param groupName Name of the new group
	 * @param groupCount Increment groupCount on success
	 * @return new group or NULL if failed to create a new group.
	 */
	J9TDFGroup *newGroup(const char *groupName, unsigned int *groupCount);

	/**
	 * Search for group with groupName in the collection
	 * @param head of the linked-list
	 * @param groupName Name of the group to look-up
	 * @return group with groupName, NULL if group with ground name not found.
	 */
	J9TDFGroup *findGroupByName(J9TDFGroup *head, const char *groupName);

	/**
	 * Add tracepoint id to end of group
	 * @param group Add tracepoint id to collection
	 * @param id Tracepoint ID
	 * @return RC_OK on success, RC_FAILED on failure
	 */
	RCType addTpIdToGroup(J9TDFGroup *group, unsigned int id);

	/**
	 * Get pre-populated group list
	 * @param groupCount Update groupCount with number of groups added.
	 * @return Linkedlist with default groups
	 */
	J9TDFGroup *getDefaultGroupSet(unsigned int *groupCount);

	/**
	 * Free all groups and associated tracepoint ids.
	 * @param head Head of the linked-list
	 * @return RC_OK on success, RC_FAILED on failure
	 */
	RCType freeGroups(J9TDFGroup *head);

	/**
	 * Output trace .h .c and .pdat files
	 * @param tdf Parsed TDF file
	 * @return RC_OK on success, RC_FAILED on failure
	 */
	RCType writeOutputFiles(J9TDFFile *tdf);

	/**
	 * Compute groups and member tracepoints
	 * @param tdf Parsed TDF file
	 * @param[out] groupCount Number of trace groups in TDF file
	 * @return On success trace groups. On failure NULL.
	 */
	J9TDFGroup *calculateGroups(J9TDFOptions *options, J9TDFFile *tdf, unsigned int *groupCount);
protected:
public:
	TraceGen()
		: _visitedFile(NULL)
	{
	}
	/**
	 * Parse TDF file and generate .h .c and .pdat files
	 * @param options Command line options
	 * @parm currentTDFFile Path to the TDF file
	 * @return RC_OK on success, RC_FAILED on failure
	 */
	RCType generate(J9TDFOptions *options, const char *currentTDFFile);

	/**
	 * Teardown Tracegen
	 */
	void tearDown();

	/**
	 * Callback called by file system iterator when TDF file is found.
	 * @param targetObject TraceGen object
	 * @param options Command line options
	 * @param tdfFileName Path to TDF file.
	 * @return RC_OK on success, RC_FAILED on failure
	 */
	static RCType generateCallBack(void *targetObject, J9TDFOptions *options, const char *tdfFileName);
};

#endif /* TRACEGEN_HPP_ */
