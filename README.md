# svelte-patcher

Solves svelte detach `node.parentNode is null` issue when user changes too quickly application state.

```
 Uncaught (in promise) TypeError: node.parentNode is null
```


if you are using vite, don't forget to delte vite cache
```
rm -rf ./node_modules/.vite
```

then
```
git clone https://github.com/comidev-france/svelte-patcher && ./svelte-patcher/svelte-patcher.c 

```


