local csv = require('csv')


function update_schedule(schedule_cache, csv_file_name)
    local group_index = __read_previous_cleaning_group_index(schedule_cache)
    local max_group_index = __read_group_number(csv_file_name)
    group_index = group_index + 1
    if group_index > max_group_index then
        group_index = 1
    end
    __save_cleaning_group_index(schedule_cache, group_index)
end


function get_today_group_name(schedule_cache, csv_file_name)
    local group_index = __read_previous_cleaning_group_index(schedule_cache)
    local group_name = __get_group_name(csv_file_name, group_index)
    return group_name
end


function get_today_group_members_string(schedule_cache, csv_file_name)
    local group_index = __read_previous_cleaning_group_index(schedule_cache)
    local group_name = __read_name_string(csv_file_name, group_index)
    return group_name
end


function __get_group_name(csv_file_name, group_index)
    local group_name, _ = __read_name_list(csv_file_name, group_index)
    return group_name
end


function __read_name_string(csv_file_name, group_index)
    local _, name_list = __read_name_list(csv_file_name, group_index)
    local name_string = __name_list_to_string(name_list)
    print(name_string)
    return name_string
end


function __read_group_number(csv_file_name)
    local f_group_list = csv.open(csv_file_name)
    assert(f_group_list ~= nil, 'Unable to open file: ' .. csv_file_name)
    local count = 0
    for fields in f_group_list:lines() do
        count = count + 1
    end
    return count
end


function __read_name_list(csv_file_name, group_index)
    local f_group_list = csv.open(csv_file_name)
    assert(f_group_list ~= nil, 'Unable to open file: ' .. csv_file_name)
    local count = 1
    local name_list = {}
    for fields in f_group_list:lines() do
        if count ~= group_index then
            goto end_loop
        end
        for i, v in ipairs(fields) do
            name_list[i] = v
        end
        :: end_loop ::
        count = count + 1
    end
    return name_list[1], {table.unpack(name_list, 2)}
end


function __name_list_to_string(name_list)
    local name_string = ''
    for i, n in ipairs(name_list) do
        if n == '' then
            goto end_loop
        end
        name_string = string.format('%s%s, ', name_string, n)
        :: end_loop ::
    end
    name_string = string.sub(name_string, 1, string.len(name_string) - 2)
    return name_string
end


function __save_cleaning_group_index(file_name, group_index)
    local fp = io.open(file_name, 'w')
    assert(fp ~= nil, 'Unable to open file: ' .. file_name)
    io.output(fp)
    io.write(group_index)
    io.close(fp)
end


function __read_previous_cleaning_group_index(file_name)
    local fp = io.open(file_name, 'r')
    assert(fp ~= nil, 'Unable to open file: ' .. file_name)
    io.input(fp)
    local index = io.read('n')
    io.close(fp)
    return index
end


--group_name, name_list = __read_name_list('group_list.csv', 3)
--print(group_name)
--for i, n in ipairs(name_list) do
--    print(i, n)
--end
--print(__name_list_to_string(name_list))