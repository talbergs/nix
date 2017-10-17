#include "remote-fs-accessor.hh"
#include "nar-accessor.hh"

namespace nix {

RemoteFSAccessor::RemoteFSAccessor(ref<Store> store, const Path & cacheDir)
    : store(store)
    , cacheDir(cacheDir)
{
    if (cacheDir != "")
        createDirs(cacheDir);
}

std::pair<ref<FSAccessor>, Path> RemoteFSAccessor::fetch(const Path & path_)
{
    auto path = canonPath(path_);

    auto storePath = store->toStorePath(path);
    std::string restPath = std::string(path, storePath.size());

    if (!store->isValidPath(storePath))
        throw InvalidPath(format("path '%1%' is not a valid store path") % storePath);

    auto i = nars.find(storePath);
    if (i != nars.end()) return {i->second, restPath};

    StringSink sink;

    Path cacheFile = cacheDir != "" ? fmt("%s/%s.nar", cacheDir, storePathToHash(storePath)) : "";

    try {
        if (cacheFile != "")
            *sink.s = nix::readFile(cacheFile);
    } catch (SysError &) { }

    if (sink.s->empty()) {
        store->narFromPath(storePath, sink);

        if (cacheFile != "")
            /* FIXME: do this asynchronously. */
            writeFile(cacheFile, *sink.s);
    }

    auto accessor = makeNarAccessor(sink.s);
    nars.emplace(storePath, accessor);
    return {accessor, restPath};
}

FSAccessor::Stat RemoteFSAccessor::stat(const Path & path)
{
    auto res = fetch(path);
    return res.first->stat(res.second);
}

StringSet RemoteFSAccessor::readDirectory(const Path & path)
{
    auto res = fetch(path);
    return res.first->readDirectory(res.second);
}

std::string RemoteFSAccessor::readFile(const Path & path)
{
    auto res = fetch(path);
    return res.first->readFile(res.second);
}

std::string RemoteFSAccessor::readLink(const Path & path)
{
    auto res = fetch(path);
    return res.first->readLink(res.second);
}

}
