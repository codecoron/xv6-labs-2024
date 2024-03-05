#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *target_file)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // read the name of each file/folder under the folder specified by fd, which is $path, name is de.name
    //目录记录了子目录和文件的信息，具体是子目录和文件的dirent结构体,包括inum和name
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        strcpy(buf, path);
        p = buf + strlen(buf); // point to the end of the path
        *p++ = '/';
        if (de.inum == 0)
            continue;
        // get the full path name of the current file/directory selected
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        // get the stat of the current file/directory
        if (stat(buf, &st) < 0)
        {
            fprintf(2, "ERROR: cannot stat %s\n", buf);
        }

        switch (st.type)
        {
        case T_FILE:
            if (strcmp(target_file, de.name) == 0)
            {
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0))
            {
                find(buf, target_file);
            }
        }
    }
    close(fd);
    return;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        fprintf(2, "Usage: find dir target\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}