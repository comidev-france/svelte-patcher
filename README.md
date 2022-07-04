# svelte-patcher

Solves svelte detach `node.parentNode is null` issue when user changes too quickly application state.

```
 Uncaught (in promise) TypeError: node.parentNode is null
```


before we start, `cd path/to/your_npm_app`


if you are using vite, don't forget to delte vite cache
```
rm -rf ./node_modules/.vite
```

then
```
git submodule add https://github.com/comidev-france/svelte-patcher && ./svelte-patcher/svelte-patcher.c 

```

should output:
```
svelte patcher v0.1
patching ./node_modules/svelte/internal/index.js...
patching ./node_modules/svelte/internal/index.mjs...
patch applied successfully
```

finally you can add the following postinstall script in your `package.json` :

```
{
...
    "scripts": {
        ...
        "postinstall" : "rm -rf node_modules/.vite && ./svelte-patcher/svelte-patcher.c"
        ...
    }
...
}
```

