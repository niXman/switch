[![Build Status](https://travis-ci.org/niXman/switch.svg?branch=master)](https://travis-ci.org/niXman/switch)

# switch
switch()-like statement whose cases are string literals and integrals

#Samples
```cpp
int i = ...;
auto r = switch_<const char*>(i)
  .case_(0, "0")
  .case_(1, "1")
  .case_(2, "2")
  .default_("undefined")
;
```

```cpp
struct point {
	int x,y;

	point(int x, int y)
		:x(x)
		,y(y)
	{}
	bool operator==(const point &r) const {
		return x == r.x && y == r.y;
	}
};

int i = ...;
auto r = switch_::switch_<point>(i)
	.case_(2, {100, 200})
	.case_(3, {200, 200})
	.default_({300, 300})
;
```

```cpp
std::string k = ...;
auto r = switch_::switch_<point>(k)
	.case_("2", {100, 200})
	.case_("3", {200, 200})
	.default_({300, 300})
;
```

```cpp
int i = ...;
auto r = switch_<int>(i)
	.case_(0, [](){return 0;})
	.case_(1, [](){return 1;})
	.case_(2, [](){return 2;})
	.default_([](){return 3;})
;
```
