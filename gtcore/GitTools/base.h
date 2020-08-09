#ifndef GT_BASE_H
#define GT_BASE_H

#include <GitTools/string.h>
#include <GitTools/exception.h>
#include <git2.h>

#include <vector>
#include <QDebug>
#include <QDateTime>

namespace git
{

    inline void check(int error)
    {
        if (error < 0)
        {
            const git_error *e = giterr_last();
            throw exception(e->message);
        }
    }

    inline void warn(int error)
    {
        if (error < 0)
        {
            const git_error *e = giterr_last();
            qDebug() << "warn: " << qt_string(e->message);
        }
    }

    class object_id
    {
    private:

        git_oid m_oid;

    public:

        object_id() = default;
        object_id(const object_id &) = default;
        object_id(object_id &&) = default;

        object_id(const git_oid *o)
        {
            if ( o ) memcpy(&m_oid, o, sizeof(m_oid));
        }

        object_id(const string &hash)
        {
            check( git_oid_fromstr(&m_oid, std_string(hash).c_str()) );
        }

        ~object_id() = default;

        object_id& operator = (const object_id &) = default;
        object_id& operator = (object_id &&) = default;

        object_id& operator = (const git_oid *o)
        {
            if ( o ) memcpy(&m_oid, o, sizeof(m_oid));
            else memset(&m_oid, 0, sizeof(m_oid));
            return *this;
        }

        bool operator == (const object_id &o) const
        {
            return memcmp(m_oid.id, o.m_oid.id, sizeof(m_oid.id)) == 0;
        }

        bool operator != (const object_id &o) const
        {
            return memcmp(m_oid.id, o.m_oid.id, sizeof(m_oid.id)) != 0;
        }

        const git_oid* data() const
        {
            return &m_oid;
        }

        string toString() const
        {
            char buf[80] {};
            if ( git_oid_tostr(buf, sizeof(buf), &m_oid) == nullptr ) return {};
            return anystr(buf);
        }

    };

    class reference
    {
    private:

        git_reference *r = nullptr;

    public:

        reference() = default;
        reference(const reference &other) = delete;
        reference(reference &&other): r(other.r)
        {
            other.r = nullptr;
        }

        reference(git_reference *ref): r(ref)
        {
        }

        ~reference()
        {
            free();
        }

        reference& operator = (const reference &) = delete;
        reference& operator = (reference &&other)
        {
            free();
            r = other.r;
            other.r = nullptr;
            return *this;
        }

        reference resolve() const
        {
            git_reference *out;
            check( git_reference_resolve(&out, r) );
            return reference(out);
        }

        bool isHead() const
        {
            return git_reference_is_branch(r) != 0 && git_branch_is_head(r) != 0;
        }

        bool isBranch() const
        {
            return git_reference_is_branch(r);
        }

        bool isRemote() const
        {
            return git_reference_is_remote(r);
        }

        bool isTag() const
        {
            return git_reference_is_tag(r);
        }

        object_id target() const
        {
            return git_reference_target(r);
        }

        string name() const
        {
            return anystr(git_reference_name(r));
        }

        string shortName() const
        {
            return anystr(git_reference_shorthand(r));
        }

        void delete_reference()
        {
            check( git_reference_delete(r) );
            free();
        }

        void delete_branch()
        {
            check( git_branch_delete(r) );
            free();
        }

        void free()
        {
            if ( r )
            {
                git_reference_free(r);
                r = nullptr;
            }
        }

    };

    class reference_info
    {
    public:

        git::string name;
        git::string short_name;
        git::object_id target;
        bool isHead = false;
        bool isBranch = false;
        bool isRemote = false;

        reference_info(const git::reference &ref):
            name(ref.name()),
            short_name(ref.shortName()),
            target(ref.resolve().target()),
            isHead(ref.isHead()),
            isBranch(ref.isBranch()),
            isRemote(ref.isRemote())
        {
        }
    };

    class signature
    {
    private:

        std::string m_name;
        std::string m_mail;
        git_time m_when;
    public:

        signature(const git_signature *s): m_name(s->name), m_mail(s->email), m_when(s->when)
        {
        }

        git_time when() const { return m_when; }
        string name() const { return anystr(m_name); }
        string mail() const { return anystr(m_mail); }

    };

    class blob
    {
    private:

        git_blob *b = nullptr;

    public:

        blob() = default;
        blob(const blob &) = delete;
        blob(blob &&other): b(other.b)
        {
            other.b = nullptr;
        }

        blob(git_blob *v): b(v)
        {
        }

        ~blob()
        {
            if ( b ) git_blob_free(b);
        }

        void operator = (blob &&other)
        {
            if ( b ) git_blob_free(b);
            b = other.b;
            other.b = nullptr;
        }

        int rawsize() const
        {
            return git_blob_rawsize(b);
        }

        QByteArray rawcontent() const
        {
            auto size = rawsize();
            if ( size >= 0 )
                return QByteArray::fromRawData(reinterpret_cast<const char*>(git_blob_rawcontent(b)), size);
            return {};
        }

    };

    class commit
    {
    private:

        git_commit *c = nullptr;

    public:

        commit() = default;
        commit(const commit &) = delete;
        commit(commit &&other): c(other.c)
        {
            other.c = nullptr;
        }

        commit(git_commit *v): c(v)
        {
        }

        ~commit()
        {
            if ( c ) git_commit_free(c);
        }

        void operator = (commit &&other)
        {
            if ( c ) git_commit_free(c);
            c = other.c;
            other.c = nullptr;
        }

        git::object_id id() const
        {
            return git_commit_id(c);
        }

        string message() const
        {
            return anystr(git_commit_message(c));
        }

        string shortMessage(int maxLen = 80) const
        {
            return message().left(maxLen).split(QRegExp("(\\r|\\n)")).first();
        }

        QDateTime dateTime() const
        {
            QDateTime dateTime;
            dateTime.setTime_t(git_commit_time(c));
            return dateTime;
        }

        signature author() const
        {
            return git_commit_author(c);
        }

        unsigned parentCount() const
        {
            return git_commit_parentcount(c);
        }

        git::object_id parentId(unsigned n) const
        {
            return git_commit_parent_id(c, n);
        }

        const git_commit* data() const
        {
            return c;
        }

    };

    class revwalk
    {
    private:

        git_revwalk *rw = nullptr;

    public:

        revwalk() = default;
        revwalk(const revwalk &) = delete;
        revwalk(revwalk &&other): rw(other.rw)
        {
            other.rw = nullptr;
        }

        revwalk(git_revwalk *v): rw(v)
        {
        }

        ~revwalk()
        {
            if ( rw ) git_revwalk_free(rw);
        }

        void operator = (revwalk &&other)
        {
            if ( rw ) git_revwalk_free(rw);
            rw = other.rw;
            other.rw = nullptr;
        }

        void setSorting(unsigned int sm)
        {
            git_revwalk_sorting(rw, sm);
        }

        void push(const object_id &o)
        {
            check( git_revwalk_push(rw, o.data()) );
        }

        void push(const char *refname)
        {
            check( git_revwalk_push_ref(rw, refname) );
        }

        void push(const QString &refname)
        {
            push(refname.toStdString().c_str());
        }

        bool next(git_oid &o)
        {
            int err = git_revwalk_next(&o, rw);
            if ( err == 0 ) return true;
            if ( err == GIT_ITEROVER ) return false;
            const git_error *e = giterr_last();
            throw exception(e->message);
        }
    };

    class tree
    {
    private:

        git_tree *t = nullptr;

    public:

        class entry
        {
        private:

            git_tree_entry *ent = nullptr;

        public:

            entry() = default;
            entry(const entry &) = delete;
            entry(entry &&other): ent(other.ent)
            {
                other.ent = nullptr;
            }

            entry(git_tree_entry *v): ent(v)
            {
            }

            ~entry()
            {
                if ( ent ) git_tree_entry_free(ent);
            }

            void operator = (entry &&other)
            {
                if ( ent ) git_tree_entry_free(ent);
                ent = other.ent;
                other.ent = nullptr;
            }

            const git_oid* id() const
            {
                return git_tree_entry_id(ent);
            }

            string name() const
            {
                return anystr(git_tree_entry_name(ent));
            }

        };

        tree() = default;
        tree(const tree &) = delete;
        tree(tree &&other): t(other.t)
        {
        }

        tree(git_tree *v): t(v)
        {
        }

        tree(const commit &commit)
        {
            check( git_commit_tree(&t, commit.data()) );
        }

        ~tree()
        {
            if ( t ) git_tree_free(t);
        }

        void operator = (tree &&other)
        {
            if ( t ) git_tree_free(t);
            t = other.t;
            other.t = nullptr;
        }

        entry entryByPath(const std::string &path) const
        {
            git_tree_entry *ent;
            check( git_tree_entry_bypath(&ent, t, path.c_str()) );
            return ent;
        }

        entry entryByPath(const QString &path) const
        {
            return entryByPath(path.toStdString());
        }

        git_tree* data() const
        {
            return t;
        }

    };

    class diff_file
    {
    private:

        const git_diff_file *f = nullptr;

    public:

        diff_file() = default;
        diff_file(const diff_file &) = default;
        diff_file(diff_file &&) = default;
        diff_file(const git_diff_file *v): f(v) { }
        ~diff_file() = default;

        string path() const
        {
            return anystr(f->path);
        }

        const git_diff_file* data() { return f; }
    };

    class delta
    {
    private:

        const git_diff_delta *d = nullptr;

    public:

        delta() = default;
        delta(const delta &) = default;
        delta(delta &&) = default;

        delta(const git_diff_delta *v): d(v) { }
        ~delta() = default;

        git_delta_t type() const
        {
            return d->status;
        }

        diff_file oldFile() const
        {
            return &d->old_file;
        }

        diff_file newFile() const
        {
            return &d->new_file;
        }

        const git_diff_delta* data() const { return d; }
    };

    class diff
    {
    private:

        git_diff *d = nullptr;

    public:

        diff() = default;
        diff(const diff &) = delete;
        diff(diff &&) = default;
        diff(git_diff *v): d(v) { }

        ~diff()
        {
            if ( d ) git_diff_free(d);
        }

        void operator = (diff &&other)
        {
            if ( d ) git_diff_free(d);
            d = other.d;
            other.d = nullptr;
        }

        unsigned deltaCount() const
        {
            return git_diff_num_deltas(d);
        }

        git::delta get_delta(unsigned n) const
        {
            return git_diff_get_delta(d, n);
        }

    };

    class repository
    {
    public:

        git_repository *r = nullptr;

    public:

        repository() = default;
        repository(const repository &) = delete;
        repository(repository &&other): r(other.r)
        {
            other.r = nullptr;
        }

        repository(const string &path)
        {
            open(path);
        }

        ~repository()
        {
            close();
        }

        void operator = (repository &&other)
        {
            close();
            r = other.r;
            other.r = nullptr;
        }

        void open(const string &path)
        {
            close();
            check( git_repository_open(&r, std_string(path).c_str()) );
        }

        void close()
        {
            if ( r == nullptr ) return;
            git_repository_free(r);
            r = nullptr;
        }

        reference get_head() const
        {
            git_reference *ref;
            check( git_repository_head(&ref, r) );
            return ref;
        }

        reference get_branch(const string &name, git_branch_t type = GIT_BRANCH_LOCAL) const
        {
            git_reference *ref;
            check( git_branch_lookup(&ref, r, std_string(name).c_str(), type) );
            return ref;
        }

        reference get_reference(const string &name)
        {
            git_reference *ref;
            check( git_reference_lookup(&ref, r, std_string(name).c_str()) );
            return ref;
        }

        blob get_blob(const git_oid *id) const
        {
            git_blob *b;
            check( git_blob_lookup(&b, r, id) );
            return b;
        }

        commit get_commit(const git_oid *id) const
        {
            git_commit *c;
            check( git_commit_lookup(&c, r, id) );
            return c;
        }

        commit get_commit(const std::string &hash) const
        {
            git_oid id;
            check( git_oid_fromstr(&id, hash.c_str()) );
            return get_commit(&id);
        }

        commit get_commit(const QString &hash) const
        {
            return get_commit(hash.toStdString());
        }

        commit get_commit(const git::object_id &id) const
        {
            return get_commit(id.data());
        }

        revwalk new_revwalk() const
        {
            git_revwalk *rw;
            check( git_revwalk_new(&rw, r) );
            return rw;
        }

        reference create_branch(const string &name, const git::commit &target, bool force = false)
        {
            git_reference *ref;
            check( git_branch_create(&ref, r, std_string(name).c_str(), target.data(), force) );
            return ref;
        }

        void delete_branch(const string &name)
        {
            delete_branch( get_branch(name) );
        }

        void delete_branch(reference &&ref)
        {
            ref.delete_branch();
        }

        void delete_refence(reference &&ref)
        {
            ref.delete_reference();
        }

        git_repository* data() const
        {
            return r;
        }

        git::diff diff(const git::tree &a, const git::tree &b)
        {
            git_diff *d;
            check( git_diff_tree_to_tree(&d, r, a.data(), b.data(), nullptr) );
            return d;
        }

    };

}

#endif // GT_BASE_H
