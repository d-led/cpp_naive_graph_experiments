#pragma once

#include <memory>
#include <vector>
#include <functional>

template<typename T>
class node : public std::enable_shared_from_this< node<T> > {
public:
    typedef std::shared_ptr< node<T> > connection_ptr;
    typedef std::vector< connection_ptr > connection_container_type;
    typedef std::weak_ptr< node<T> > reference_type;
    typedef std::vector< reference_type > references_type;
    typedef std::function<void(T const&)> on_destructor_type;

private:
    node(T const& t,on_destructor_type on_d):
    id(t),
    on_destructor(on_d)
    {}

private:
    void reference(connection_ptr connection) {
        references.push_back(connection);
    }

public:
    ~node() {
        if (on_destructor)
            on_destructor(this->get_id());
    }

public:
    static connection_ptr create(T const& t,on_destructor_type on_d = on_destructor_type()) {
        return connection_ptr( new node<T>(t,on_d) );
    }

public:
    connection_container_type get_connections() const {
        connection_container_type connections;
        for (auto reference: references)
            if (auto connection = reference.lock())
               connections.push_back(connection);

        return connections;
    }

    T get_id() const {
        return id;
    }

public:
    connection_ptr add(T const& t) {
        auto connection( create(t,on_destructor) );
        return add( connection );
    }

    connection_ptr add(connection_ptr connection) {
        this->reference( connection );

        // undirected graph
        connection->reference( this->shared_from_this() );
        return connection;
    }

private:
    T id;
    references_type references;
    on_destructor_type on_destructor;
};
