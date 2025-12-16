import random
import datetime

# ================= 配置区域 =================
TOTAL_RECORDS = 1000

# 航空公司
airline_map = {
    "中国国航": "CA", "东方航空": "MU", "南方航空": "CZ", "海南航空": "HU",
    "厦门航空": "MF", "四川航空": "3U", "深圳航空": "ZH", "春秋航空": "9C"
}

# 城市
cities = [
    "北京", "上海", "广州", "深圳", "成都", "杭州", "西安",
    "重庆", "昆明", "南京", "长沙", "海口", "乌鲁木齐", "三亚",
    "哈尔滨", "青岛", "大连", "天津", "郑州", "武汉", "江门", "合肥", "抚州"
]

# --- 修改点 1：降低取消率 ---
# 计划中占 20 份，延误 1 份，取消 1 份
status_list = ["计划中"] * 20 + ["延误"] + ["取消"]


# ================= 生成逻辑 =================

def generate_and_sort_data():
    raw_data = []
    sql_statements = []

    # --- 修改点 2：定义一个集合，用来记录已经生成过的航班号 ---
    used_flight_ids = set()

    print("正在生成并排序数据...\n")

    for i in range(TOTAL_RECORDS):
        # 1. 随机选航司
        airline_name, airline_code = random.choice(list(airline_map.items()))

        # --- 修改点 3：死循环确保航班号唯一 ---
        while True:
            # 生成临时航班号
            temp_id = f"{airline_code}{random.randint(1000, 9999)}"
            # 检查是否重复
            if temp_id not in used_flight_ids:
                # 没重复，记录下来，并跳出循环
                used_flight_ids.add(temp_id)
                flight_id = temp_id
                break
            # 如果重复了 (in used_flight_ids)，while 循环会继续，重新生成

        # 2. 随机起降城市
        dep_city = random.choice(cities)
        arr_city = random.choice(cities)
        while dep_city == arr_city:
            arr_city = random.choice(cities)

        # 3. 生成时间 (未来 1~45 天内)
        now = datetime.datetime.now()
        random_days = random.randint(1, 45)
        random_hour = random.randint(6, 23)
        random_minute = random.choice([0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55])

        dep_time_obj = now + datetime.timedelta(days=random_days)
        dep_time_obj = dep_time_obj.replace(hour=random_hour, minute=random_minute, second=0, microsecond=0)

        # 4. 飞行时间 & 到达时间
        duration_minutes = random.randint(60, 270)
        arr_time_obj = dep_time_obj + datetime.timedelta(minutes=duration_minutes)

        # 5. 价格
        base_price = duration_minutes * 3
        price = base_price + random.randint(-100, 300)
        price = int(round(price, -1))
        if price < 200: price = 200

        # 6. 座位
        capacity = random.choice([150, 168, 180, 200, 320])
        occupancy_rate = random.uniform(0.3, 0.95)
        booked_count = int(capacity * occupancy_rate)

        # 7. 状态
        status = random.choice(status_list)

        # 存入列表
        raw_data.append({
            "flight_id": flight_id,
            "airline": airline_name,
            "dep_city": dep_city,
            "arr_city": arr_city,
            "dep_time_obj": dep_time_obj,
            "arr_time_obj": arr_time_obj,
            "price": price,
            "capacity": capacity,
            "booked_count": booked_count,
            "status": status
        })

    # --- 排序 ---
    raw_data.sort(key=lambda x: x["dep_time_obj"])

    # --- 输出 ---
    print(
        f"{'序号':<6}{'航班号':<10}{'航空公司':<10}{'出发地':<8}{'目的地':<8}{'起飞时间':<20}{'到达时间':<20}{'价格':<8}{'状态':<8}")
    print("-" * 110)

    for idx, item in enumerate(raw_data):
        dep_str = item["dep_time_obj"].strftime("%Y-%m-%d %H:%M:%S")
        arr_str = item["arr_time_obj"].strftime("%Y-%m-%d %H:%M:%S")

        # 控制台打印
        print(
            f"{idx + 1:<6}{item['flight_id']:<10}{item['airline']:<10}{item['dep_city']:<8}{item['arr_city']:<8}{dep_str:<20}{arr_str:<20}{item['price']:<8}{item['status']:<8}")

        # SQL 生成
        sql = (
            f"INSERT INTO flights "
            f"(flight_id, airline, departure_city, arrival_city, departure_time, arrival_time, price, capacity, booked_count, status) "
            f"VALUES ('{item['flight_id']}', '{item['airline']}', '{item['dep_city']}', '{item['arr_city']}', "
            f"'{dep_str}', '{arr_str}', {item['price']}, {item['capacity']}, {item['booked_count']}, '{item['status']}');"
        )
        sql_statements.append(sql)

    return sql_statements


if __name__ == "__main__":
    sqls = generate_and_sort_data()

    print("-" * 110)
    print(f"数据生成完毕。")

    filename = "flight_data.sql"
    try:
        with open(filename, "w", encoding="utf-8") as f:
            create_table_sql = """
DROP TABLE IF EXISTS flights;
CREATE TABLE flights (
    flight_id VARCHAR(10) NOT NULL PRIMARY KEY COMMENT '航班号',
    airline VARCHAR(50) COMMENT '航空公司',
    departure_city VARCHAR(50) COMMENT '出发城市',
    arrival_city VARCHAR(50) COMMENT '到达城市',
    departure_time DATETIME COMMENT '起飞时间',
    arrival_time DATETIME COMMENT '到达时间',
    price DECIMAL(10, 2) COMMENT '票价',
    capacity INT COMMENT '总座位数',
    booked_count INT COMMENT '已订票数',
    status VARCHAR(20) COMMENT '航班状态'
);
"""
            f.write(create_table_sql + "\n")
            f.write("-- 开始插入模拟数据 (已去重)\n")
            for sql in sqls:
                f.write(sql + "\n")

        print(f"\n✅ 成功生成文件: {filename}")
        print("💡 之前的重复主键问题已解决，请直接运行此文件。")

    except Exception as e:
        print(f"❌ 保存文件失败: {e}")