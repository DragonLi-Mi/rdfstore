
#ifndef COMMAND_H
#define COMMAND_H
#include "../libgist/gist.h"
void CommandCreate(const char *table);
void CommandCheck(const char *table);
void CommandDrop(const char *table);
void CommandOpen(const char *table);
void CommandClose(const char *table);
void CommandSelect(const char *table, void *query);
void CommandDelete(const char *table, void *query);
void CommandInsert(const char *table, void * key, int klen, void *data, int dlen);
void CommandQuit();
void CommandDump(const char *table, shpid_t pgno);
void CommandCreateNodefile(const char *table);
long long  add2Node(const char *node );

extern void printDatum(
//void printDatum(
    void *key,
    int klen,
    void *data,
    int dlen);

extern char *identStr;
//char *identStr = "integer B-tree";
extern gist_ext_t *extension;
//gist_ext_t *extension = &bt_int_ext;

#endif // COMMAND_H




