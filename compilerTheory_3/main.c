#define _CRT_SECURE_NO_WARNINGS

#include "analyzer.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


int next = 0;
bool flag = false;
bool check = false;
bool isCondition = false;
bool isOperator = true;
bool isAndOr = false;
int scopeAfterAndOr = 0;
int scopeAfterCondition = 0;
int deep = 0;
extern size;
extern yyin;
extern *tokenArr;

char* tokens[] = {
        "SQLend", "SQLmore", "SQLname", "SQLscope", "SQLescope", "SQLdigit", "SQLstring",
        "SQLoperator", "SQLequals", "SQLcondition",
        "SQLshowtables", "SQLshowdatabases", "SQLshowcolumnsfrom",
        "SQLselect", "SQLfrom", "SQLselectdistinct",
        "SQLlimit", "SQLoffset",
        "SQLorderby",
        "SQLwhere", "SQLbetweenand", "SQLandor", "SQLin", "SQLnotin", "SQLlike",
        "SQLfunc",
        "SQLupdate", "SQLset", "SQLdelete", "SQLinsertinto", "SQLvalues",
        "SQLcreatetable", "SQLnewcolumn"
};

bool term(tokenType token) {
	if (next >= size) {
		return false;
	}
	return token == tokenArr[next++];
}

bool searchColumns();
bool searchCondition();
bool searchInWhere();
bool searchWhereCondition();
bool startParse();
bool searchInSet();

bool searchNewTable() {
	int save = next;
	if(term(SQLnewcolumn)){
		return searchNewTable();
	}
	next = save;
	return true;
}

bool searchMore() {
	int save = next;
	if (term(SQLmore)) {
		return searchInSet();
	}
	next = save;
	if (term(SQLwhere)) {
		return searchInWhere();
	}
}

bool searchInSet() {
	int save = next;
	if (term(SQLname) && term(SQLequals)) {
		save = next;
		if (term(SQLdigit)) {
			return searchMore();
		}
		next = save;
		if (term(SQLstring)) {
			return searchMore();
		}
	}
}

bool parseSelect() {
	bool res = searchColumns();
	if (res) {
		return startParse();
	}
	else {
		return false;
	}
}

bool startParse() {
	if (next >= size) {
		return true;
	}
	int save = next;
	if (term(SQLselect)) {
		return parseSelect();
	}
	next = save;
	if (term(SQLshowdatabases) && term(SQLend)) {
		return startParse();
	}
	next = save;
	if (term(SQLshowtables) && term(SQLend)) {
		return startParse();
	}
	next = save;
	if (term(SQLshowcolumnsfrom) && term(SQLname) && term(SQLend)) {
		return startParse();
	}
	next = save;
	if (term(SQLcreatetable) && term(SQLscope) && searchNewTable() && term(SQLescope) && term(SQLend)){
		return startParse();
	}
	next = save;
	if (term(SQLdelete) && term(SQLfrom) && term(SQLname)) {
		if (term(SQLwhere)) {
			return searchInWhere();
		}
		next = save;
		if (term(SQLend)) {
			return startParse();
		}
		else {
			return false;
		}
	}
	next = save;
	if (term(SQLupdate) && term(SQLset)) {
		return searchInSet();
	}
	return false;
}


bool searchCondition() {
	int save = next;
	if (term(SQLfrom) && term(SQLname))  {
		if (deep == 0) {
			if (term(SQLwhere)) {
				return searchInWhere();
			}
			next = save;
			if (term(SQLend)) {
				return true;
			}
		}
	}
	next = save;
	if (term(SQLmore)) {
		return searchColumns();
	}
	next = save;
	if (term(SQLoperator)) {
		return searchColumns();
	}
	next = save;
	if (term(SQLescope)) {
		if (--deep >= 0) {
			return searchCondition();
		}
		else {
			return false;
		}
	}
	next = save;
	if (term(SQLend)) {
		return true;
	}
	next = save;
	if (term(SQLwhere)) {
		return searchInWhere();
	}
	return false;
}

bool searchColumns() {
	int save = next;
	if (term(SQLname)) {
		return searchCondition();
	}
	next = save;
	if (term(SQLdigit)) {
		return searchCondition();
	}
	next = save;
	if (term(SQLscope)) {
		deep++;
		return searchColumns();
	}
	next = save;
	if (term(SQLfunc) && term(SQLscope)) {
		deep++;
		return searchColumns();
	}
	return false;
}


bool searchWhereCondition() {
	int save = next;
	if (term(SQLoperator)) {
		if (isOperator) {
			return searchInWhere();
		}
		else {
			return false;
		}
	}
	next = save;
	if (term(SQLescope)) {
		if (--deep >= 0) {
			if (scopeAfterAndOr <= 0 && isAndOr) {
				return false;
			}
			else {
				scopeAfterAndOr--;
			}
			if (isCondition) {
				if (scopeAfterCondition == 0) {
					isOperator = false;
				}
				else {
					scopeAfterCondition--;
					isOperator = true;
				}
			}
			return searchWhereCondition();
		}
		else {
			return false;
		}
	}
	next = save;
	if (term(SQLcondition)) {
		isCondition = true;
		isAndOr = false;
		scopeAfterCondition = 0;
		if (!flag) {
			flag = true;
			return searchInWhere();
		}
		else {
			return searchInWhere();
			//return false;
		}
	}
	next = save;
	if (term(SQLandor)) {
		scopeAfterAndOr = 0;
		isOperator = true;
		isAndOr = true;
		if (deep > 0 && isCondition) {
			check = true;
		}
		flag = false;
		isCondition = false;
		return searchInWhere();
	}
	next = save;
	if (term(SQLend) && deep == 0 && isCondition) {
		return startParse();
	}
	return false;
}

bool searchInWhere() {
	int save = next;
	if (term(SQLname)) {
		return searchWhereCondition();
	}
	next = save;
	if (term(SQLdigit)) {
		return searchWhereCondition();
	}
	next = save;
	if (term(SQLscope)) {
		save = next;
		deep++;
		if (isAndOr) {
			scopeAfterAndOr++;
		}
		if (isCondition) {
			scopeAfterCondition++;
		}
		if (deep == 1 && term(SQLselect)) {
			return searchColumns();
		}
		next = save;
		return searchInWhere();
	}
	next = save;
	if (term(SQLfunc) && term(SQLscope)) {
		deep++;	
		return searchInWhere();
	}
	return false;
}


int main(int argc, char** argv) {
	analyze();
	printf("%d\n", size);
	if (startParse()) {
		printf("CORRECT\n");
		for (int i = 0; i < size; i++)
		    printf("%s ", tokens[tokenArr[i]]);
	}
	else {
		printf("ERROR");
	}
}