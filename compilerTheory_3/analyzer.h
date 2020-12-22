#pragma once
#ifndef __ANALYZER_H__
#define __ANALYZER_H__

typedef enum tokenType {
	SQLend, SQLmore, SQLname, SQLscope, SQLescope, SQLdigit, SQLstring,
	SQLoperator, SQLequals, SQLcondition,
	SQLshowtables, SQLshowdatabases, SQLshowcolumnsfrom,
	SQLselect, SQLfrom, SQLselectdistinct,
	SQLlimit, SQLoffset,
	SQLorderby,
	SQLwhere, SQLbetweenand, SQLandor, SQLin, SQLnotin, SQLlike,
	SQLfunc,
	SQLupdate, SQLset, SQLdelete, SQLinsertinto, SQLvalues,
	SQLcreatetable, SQLnewcolumn
}tokenType;



void addToken(tokenType token);

void analyze();

#endif