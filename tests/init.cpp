#include <rbt.hpp>
#include <catch.hpp>

SCENARIO("NULL")
{
	RBT<int> a;
	REQUIRE(a.Empty());
}
SCENARIO("add")
{
	RBT <int> a;
	a.Insert(5);
	REQUIRE(!a.Empty());
}
SCENARIO("BST del non inserted element", "[delete]")
{
	RBT<int> tree = { 6 } ;
	tree.Remove(3);
	REQUIRE(!tree.Empty());
}
SCENARIO("BST del root with one element", "[delete]")
{
	RBT<int> tree = { 6 };
	tree.Remove(6);
	REQUIRE(tree.Empty());
}
SCENARIO("BST del root without child", "[delete]")
{
	RBT<int> tree = { 6, 4, 3 };
	tree.Remove(6);
	RBT<int> tr ={ 4, 3 };
	REQUIRE(tree ==tr);
}
SCENARIO("BST del root with two children", "[delete]")
{
	RBT<int> tree = { 6, 4, 3, 9, 8, 12, 10, 11 };
	tree.Remove(4);
	RBT<int>tr={ 8, 6,  10, 3, 9, 12, 11 };
	REQUIRE(tree ==tr );
}
SCENARIO("BST del non root without children", "[delete]")
{
	RBT<int> tree = { 6, 4, 3, 9, 8, 12, 10, 11 };
	tree.Remove(3);
	RBT<int>tr= { 8,  4, 10, 9, 11, 12, 6 };
	REQUIRE(tree == tr);
}
SCENARIO("BST del non root with one child", "[delete]")
{
	RBT<int> tree = { 6, 4, 3, 9, 8, 12, 10, 11 };
	tree.Remove(11);
	RBT<int> tr ={ 8,  4, 10, 9, 4, 12, 6, 3};
	REQUIRE(tree == tr);
}
