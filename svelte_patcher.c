//usr/bin/cc -g "$0" && exec ./a.out "$0" && rm ./a.out

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

const char *detach_old =
"function detach(node) {\n"
"    node.parentNode.removeChild(node);\n"
"}\n";



const char *detach_new =
"function detach(node) {\n"
"\tif (typeof node === 'undefined' ||  node == null)\n"
"\t\treturn console.log('[svelte_patch warning] detaching"
"an invalid node: ', node);\n"
"\tif (typeof node.parentNode === 'undefined' || node.parentNode === null)\n"
"\t\treturn console.log('[]svelte_patch warning] detaching"
"a node with invalid parent: ', node.parentNode);\n"
"\tnode.parentNode.removeChild(node);\n"
"}\n";

const char *nav_old =
"const route = await runHooksBeforeUrlChange(event, url)\n";

const char *nav_new =
"let route;\ntry {\n"
"\troute = await runHooksBeforeUrlChange(event, url)\n"
"}\ncatch(ex){\n"
"console.log('roxy navigator.js warning : ', ex.message);\n"
"console.log('route : ', route);\n"
"route = url;"
//"route = null;\n"
"}\n";

int illiad(void *addr)
{
    printf("freeing: %p\n", addr);
    free(addr);
    return (1);
}

int patch_once(const char *file_path, const char *old, size_t old_len, const char *new, size_t new_len)
{
    int     fd;
    char    *str;
    char    *str_p;
    char    *patch_addr;
    size_t  fsize;
    int     found;
    size_t     ri;

    printf("patching %s...\n", file_path);
   found = 0;
   fd = open(file_path, O_RDONLY);
   if (fd < 0)
        return !!printf("[svelte_patcher line=%i] Unable to open(%s, O_RDONLY)\n", __LINE__, file_path);
   fsize = lseek(fd, 0, SEEK_END);
   close(fd);
   fd = open (file_path, O_RDONLY);
   if (fd < 0)
        return !!printf("[svelte_patcher line=%i] Unable to open(%s, O_RDONLY)\n", __LINE__, file_path);
   str_p = str = malloc(fsize);
   if (fsize <= 0 || str == 0 || (ri = read(fd, str, fsize)) < old_len && ri < 0)
        return !!printf("[svelte_patcher line=%i] Unable to alloc or read(%i, %p, %zi) %zi %p\n", __LINE__, fd, str_p, fsize, ri, str_p)
            && illiad(str_p);
   str[fsize] = 0;
   close(fd);
   fd = open(file_path, O_WRONLY | O_TRUNC) ;
   if (fd < 0)
        return !!printf("[svelte_patcher line=%i] Unable to open(%s, O_WRDONLY | O_TRUNC)\n", __LINE__, file_path)
            && illiad(str_p);
   while (*str)
   {
        if (!strncmp(str, old, old_len))
        {
            if (write(fd, new, new_len) < 0)
                return !!printf("[svelte_patcher line=%i] Unable to write(%i, %s, %zi)\n", __LINE__, fd, new, new_len) && illiad(str_p);
            str += old_len - 1;
            found = 1;
        }
        else
            if (write(fd, str, 1) < 0)
                return !!printf("[svelte_patcher line=%i] Unable to write(%i, %c, 1)\n", __LINE__, fd, *str)
                    && illiad(str_p);
        str += 1;
   }
    if (!found)
        return !!printf("[svelte_patcher line=%i] Unable to find the string to patch! old=%s\n", __LINE__, old)
            && illiad(str_p);
   free(str_p);
   close(fd);
   return 0;
}

int main()
{
    size_t  detach_old_len;
    size_t  detach_new_len;
    size_t  nav_old_len;
    size_t  nav_new_len;

    detach_old_len = strlen(detach_old);
    detach_new_len = strlen(detach_new);
    nav_old_len = strlen(nav_old);
    nav_new_len = strlen(nav_new);

    printf("svelte patcher v0.1\n");
    if (patch_once("./node_modules/svelte/internal/index.js", detach_old, detach_old_len, detach_new, detach_new_len))
        return 1;
    if (patch_once("./node_modules/svelte/internal/index.mjs", detach_old, detach_old_len, detach_new, detach_new_len))
        return 1;
  if (patch_once("./node_modules/@roxi/routify/runtime/navigator.js", nav_old, nav_old_len, nav_new, nav_new_len))
        return 1;
    printf("patch applied successfully\n");
    return 0;
}
