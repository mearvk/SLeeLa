package com.mearvk.sleela.skya;

import java.util.ArrayList;
import java.util.List;

final class SkyaGroupManager {
    record Group(String name, String owner, String host, String address, String room) {}

    private final List<Group> groups = new ArrayList<>();
    static final int MAX_GROUPS = 100;

    synchronized List<Group> all() { return List.copyOf(groups); }
    synchronized boolean add(Group g) {
        if (groups.size() >= MAX_GROUPS) return false;
        if (g == null || g.name() == null || g.name().isBlank()) return false;
        if (groups.stream().anyMatch(x -> x.name().equalsIgnoreCase(g.name()))) return false;
        groups.add(g);
        return true;
    }
    synchronized void remove(String name) { groups.removeIf(x -> x.name().equalsIgnoreCase(name)); }

    synchronized List<Group> search(String query) {
        String q = query == null ? "" : query.trim().toLowerCase();
        if (q.isEmpty()) return all();
        return groups.stream().filter(g ->
                contains(g.name(), q) || contains(g.owner(), q) ||
                contains(g.host(), q) || contains(g.address(), q) || contains(g.room(), q)
        ).toList();
    }

    private static boolean contains(String value, String q) {
        return value != null && value.toLowerCase().contains(q);
    }
}
