#pragma once

template <class DataType, class TableType>
class rule_controller {
protected:
    TableType table;
    name self;
    name rule_name;

public:
    rule_controller(name _self, name _rule_name)
            : table(_self, _self.value)
            , self(_self), rule_name(_rule_name) { }

    void create_rules(const std::vector<DataType> &data, bool truncate) {
        require_auth(self);

        if (truncate) {
            truncate_rules(9999);
        }

        for (auto iter = data.begin(); iter != data.end(); ++iter) {
            auto pos = table.find(iter->primary_key());
            if (pos != table.cend()) {
                table.modify(pos, self, [&](auto &rule) {
                    rule = *iter;
                });
            } else {
                table.emplace(self, [&](auto &rule) {
                    rule = *iter;
                });
            }
        }
    }

    void truncate_rules(uint16_t size) {
        require_auth(self);

        auto iter = table.begin();
        uint16_t count = 0;
        while (iter != table.cend()) {
            iter = table.erase(iter);
            count++;
            if (count == size) {
                break;
            }
        }
    }

    bool is_empty() {
        return table.cbegin() == table.cend();
    }

    TableType& get_table() {
        return table;
    }
};
