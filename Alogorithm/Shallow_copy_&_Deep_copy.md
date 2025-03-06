리스트 자체를 대입할 때 의도한 대로 복사가 이뤄지지 않는다.

의도한대로 복사를 하려면 얕은 복사와 깊은 복사 방법을 사용해야한다.

```Python
def modify_graph(graph):
    graph[0][0] = 999
    return graph

original = [[1, 2], [3, 4]]
result = modify_graph(original)

print(original)  # [[999, 2], [3, 4]]
print(result)    # [[999, 2], [3, 4]]
print(original is result)  # True
```

## 얕은 복사

- 주소 값을 복사

- 복사한 객체가 변경된다면 기존의 객체도 변경된다.

> 동일한 주소를 참조하기 때문이다.

```Python
def modify_graph_with_copy(graph):
    graph_copy = [row[:] for row in graph]  # 얕은 복사
    graph_copy[0][0] = 999
    return graph_copy

original = [[1, 2], [3, 4]]
result = modify_graph_with_copy(original)

print(original)  # [[1, 2], [3, 4]]
print(result)    # [[999, 2], [3, 4]]
print(original is result)  # False
```

- 얕은 복사를 통해 새로운 객체를 생성하고 해당 복사본을 수정하게 될 경우 원본 객체는 변경되지 않는다. 

### 얕은 복사의 문제점

- 리스트의 원소가 다차원리스트인 경우, 그 값이 변경될 때 원본도 같이 변경된다.

 ```Python
x = [1,2,3,[4]]
y = x.copy()
y[3][0] = 5
# 원본 객체도 동일하게 변경된다.
print(x) #[1,2,3,[5]]
print(y) #[1,2,3,[5]]

# 일반 값은 원본 객체가 변경되지 않는다.
y[1] = 6
print(x) #[1,2,3,[5]]
print(y) #[1,6,3,[5]]
```

## 깊은 복사 

- '실제 값'을 새로운 메모리에 복사

- 주로 copy 객체 import 를 사용하여 복사한다.

- 객체 주소를 복사하지 않고 객체 자체인 모든 것을 복사하므로 속도나 성능에 좋지 않을 수 있다.

```Python
import copy
a = [1,2,[3]]
b = copy.deepcopy(a)
b[2][0] = 4
print(a) #[1, 2, [3]]
print(b) #[1, 2, [4]]

b[0] = 5
print(a) #[1, 2, [3]]
print(b) #[5, 2, [4]]

```

-----

## 예제 문제

[감시_15683](https://www.acmicpc.net/workbook/view/8708)

```Python
import sys
input = sys.stdin.readline

N, M = map(int, input().split())
graph = [list(map(int, input().split())) for _ in range(N)]
min_x = float('inf')

def count_0(graph):
    global min_x
    now_sum = sum(row.count(0) for row in graph)
    min_x = min(min_x, now_sum)

def cctv(x, y, graph_now, dir):
    # 얕은 복사
    graph_new = [row[:] for row in graph_now]
    dx = [-1, 0, 1, 0]
    dy = [0, 1, 0, -1]
    nx, ny = x + dx[dir], y + dy[dir]
    while 0 <= nx < N and 0 <= ny < M and graph_new[nx][ny] != 6:
        if graph_new[nx][ny] == 0:
            graph_new[nx][ny] = 7
        nx, ny = nx + dx[dir], ny + dy[dir]
    return graph_new

def cctv_act(type, x, y, graph_now, dd):
    # 얕은 복사
    graph_next = [row[:] for row in graph_now]
    if type == 1:
        graph_next = cctv(x, y, graph_next, dd)
    elif type == 2:
        graph_next = cctv(x, y, graph_next, dd)
        graph_next = cctv(x, y, graph_next, (dd + 2) % 4)
    elif type == 3:
        graph_next = cctv(x, y, graph_next, dd)
        graph_next = cctv(x, y, graph_next, (dd + 1) % 4)
    elif type == 4:
        for i in range(3):
            graph_next = cctv(x, y, graph_next, (dd + i) % 4)
    return graph_next

def cctv_5(x, y, graph_now):
    # 얕은 복사
    graph_next = [row[:] for row in graph_now]
    for i in range(4):
        graph_next = cctv(x, y, graph_next, i)
    return graph_next 

def activate(cctv_list, graph_now):
    if not cctv_list:
        count_0(graph_now)
        return
    
    cctv_now = cctv_list.pop()
    x, y, cctv_type = cctv_now
    
    directions = 4 if cctv_type in [1, 3, 4] else 2 if cctv_type == 2 else 1
    
    for i in range(directions):
        graph_next = cctv_act(cctv_type, x, y, graph_now, i)
        activate(cctv_list.copy(), graph_next)
    
    cctv_list.append(cctv_now)

cctv_list = []
for i in range(N):
    for j in range(M):
        if graph[i][j] == 5:
            graph = cctv_5(i, j, graph)
        elif graph[i][j] in [1, 2, 3, 4]:
            cctv_list.append([i, j, graph[i][j]])

activate(cctv_list, graph)
print(min_x)
```

- 해당 문제에서 얕은 복사를 사용하지 않으면 각각의 그래프가 적절하게 변경되지 않는다.
