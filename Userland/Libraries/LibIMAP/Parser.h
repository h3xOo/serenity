/*
 * Copyright (c) 2021, Kyle Pereira <hey@xylepereira.me>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteBuffer.h>
#include <AK/Result.h>
#include <LibIMAP/Objects.h>

namespace IMAP {
class Client;

struct ParseStatus {
    bool successful;
    Optional<Response> response;
};

class Parser {
public:
    ParseStatus parse(ByteBuffer&& buffer, bool expecting_tag);

private:
    // To retain state if parsing is not finished
    ByteBuffer m_buffer;
    SolidResponse m_response;
    unsigned position { 0 };
    bool m_incomplete { false };
    bool m_parsing_failed { false };

    bool try_consume(StringView);
    bool at_end() { return position >= m_buffer.size(); };

    void parse_response_done();

    void consume(StringView x);

    unsigned parse_number();
    Optional<unsigned> try_parse_number();

    void parse_untagged();

    StringView parse_atom();
    StringView parse_quoted_string();
    StringView parse_string();
    Optional<StringView> parse_nstring();

    ResponseStatus parse_status();

    template<typename T>
    Vector<T> parse_list(T (*converter)(StringView));

    static MailboxFlag parse_mailbox_flag(StringView s);

    StringView parse_while(Function<bool(u8)> should_consume);

    void parse_capability_response();

    ListItem parse_list_item();

    StringView parse_literal_string();
    StringView parse_astring();
};
}
