# Topcoder Marathon Match 109: RoadNetwork

<https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17427&compid=81910>

## 問題

制約が増えた Steiner 最小森問題。

無向グラフ G = (V, E) が与えられる。
辺には重みとして費用 M : V → ℕ と価値 P : V → ℕ が定まっている。
連結であり、自己ループや多重辺はない。

さらに G 上のルートが R 個与えられる。
ただしルートとは頂点の対 (A, B) であって、
それぞれのルート  には価値 P が定まっている。

予算 NM が与えられる。
費用の総和がこれを越えないように辺を選んで部分グラフを作れ。
特に、部分グラフに含まれる辺の価値の総和 P1 とルートの価値の総和 P2 の積 P1 P2 をできるだけ大きくするものがよい。


## 日記

### 2019/03/16 12:00

入力の制約や傾向によって方針が変わりそうな問題に見える。
とりあえず貪欲を書きたい。
辺の費用順、辺のコスパ順、ルートの費用順、ルートのコスパ順、などなど。

### 2019/03/17 03:00

実装した。

### 2019/03/17 21:00

集計をした。辺とルートそれぞれについて (得た点数 / 点数の総和) の平均値。

1.  辺のランダム順
1.  辺のコスパ順
1.  ルートの費用順
1.  ルートの費用順 + 余りで辺のコスパ順
1.  ルートのコスパ順 ((辺評価の総和 + ルート評価) / 辺費用の総和)

``` console
$ COMMITS='... 20673a651715e1226363c8910012077444081827'
$ for commit in $COMMITS ; do git checkout ${commit} RoadNetwork.cpp ; cxx main.cpp -o bin/$commit ; done
$ for commit in $COMMITS ; do for i in $(seq 1000) ; do java -jar tester.jar -exec ./bin/$commit -seed ${i}000 -novis -json > log/${commit}.${i}.json ; done ; done
$ for commit in $COMMITS ; do for i in $(seq 2000) ; do cat log/${commit}.${i}.json ; done | jq --slurp '[ ([ .[] | .details.edgeScore / ([ .edges[] | .[3] ] | add) ] | add / length), ([ .[] | .details.routeScore / ([ .routes[] | .[2] ] | add) ] | add / length) ]' ; done
[
  0.12896243752421782,
  0.0004693963686985877
]
[
  0.21155468186470044,
  0.0006930874832891006
]
[
  0.12095579870967947,
  0.5571152809237752
]
[
  0.13422784723756195,
  0.5571152809237752
]
[
  0.1436605393105044,
  0.6228213963223839
]
```

辺のコスパ順でも 0.2 程度と、意外と辺の重みが使えないことが分かる。
これは単純なナップサック問題なので、ほぼ上限値と言ってよいだろう。

ルートの上限は未知数。

### 2019/03/18 00:00

ひとつめに使うルートを総当たりする解を書いた。

### 2019/03/18 01:00

フローがありそうな気がしてきた。
無向なのでフローではないけど、雰囲気は近いので見ておくことはあり。
答えを決め打ちしての最小費用流とか。
難しそう？

適当な根を決めて有向グラフ化するのはどうか。
これはルートの取り扱いを面倒にするだけに見える。

ルート間の相互関係を取り扱えていない。
ルートの構築に寄与するような辺にボーナスを与え、その順に取る貪欲はどうか。
複数のルートの共通部分ルートとなるようなルートを作り、これをルート集合に足すのはどうか。

そろそろ焼きなましやビームサーチ方向への展開を初める必要があるのではないか。

### 2019/03/18 02:50

とりあえず提出。
ルートのコスパ順 ((辺評価の総和 + ルート評価) / 辺費用の総和) のもの `20673a651715e1226363c8910012077444081827` を使った。
ひとつめのルートを総当たりする解はまだ評価が済んでいないのとバグが残ってそうだったため。

<details>
<summary>順位表</summary>

| Handle           | Score      | Rank  | Last Submission Time  | Language  | Example Tests  | Submissions  |
|------------------|------------|-------|-----------------------|-----------|----------------|--------------|
| wleite           | 981684.78  | 1     | 03.17.2019 13:35:50   | Java      | 6              | 6            |
| EvbCFfp1XB       | 932706.34  | 2     | 03.17.2019 09:24:29   | Java      | 2              | 2            |
| mugurelionut     | 921880.78  | 3     | 03.17.2019 11:33:56   | C++       | 11             | 4            |
| yowa             | 919719.61  | 4     | 03.17.2019 11:18:16   | C++       | 3              | 3            |
| iwashi31         | 910250.86  | 5     | 03.17.2019 10:24:25   | C++       | 9              | 6            |
| nika             | 904174.56  | 6     | 03.16.2019 16:19:35   | C++       | 2              | 1            |
| kotamanegi       | 892005.72  | 7     | 03.17.2019 04:41:56   | C++       | 4              | 3            |
| iehn             | 850766.30  | 8     | 03.17.2019 08:44:16   | C++       | 15             | 3            |
| gasin            | 837532.04  | 9     | 03.16.2019 22:09:25   | C++       | 2              | 2            |
| kimiyuki         | 824296.54  | 10    | 03.17.2019 13:42:36   | C++       | 1              | 1            |
| kosakkun         | 819520.83  | 11    | 03.17.2019 11:34:26   | C++       | 4              | 4            |
| HrKdr            | 790451.74  | 12    | 03.17.2019 13:19:51   | C++       | 2              | 2            |
| tanzaku          | 788267.69  | 13    | 03.17.2019 06:35:55   | C++       | 2              | 2            |
| windhunterSB     | 772374.70  | 14    | 03.17.2019 11:25:38   | C++       | 3              | 3            |
| yuya178          | 759954.53  | 15    | 03.17.2019 02:08:29   | Java      | 12             | 4            |
| my316g           | 709340.16  | 16    | 03.17.2019 10:53:07   | C++       | 1              | 1            |
| vi002            | 684057.50  | 17    | 03.17.2019 11:41:49   | C++       | 2              | 2            |
| ts__             | 655237.71  | 18    | 03.17.2019 12:32:38   | C++       | 2              | 2            |
| nel215           | 651648.95  | 19    | 03.17.2019 04:11:07   | C++       | 1              | 1            |
| seess_yshysj     | 568583.98  | 20    | 03.16.2019 21:23:06   | C++       | 8              | 3            |
| kishore_g84      | 523491.10  | 21    | 03.17.2019 02:23:53   | Java      | 1              | 2            |
| ebicochineal     | 477448.36  | 22    | 03.17.2019 05:51:20   | C++       | 1              | 3            |
| bduvenhage       | 459662.13  | 23    | 03.17.2019 02:52:33   | C++       | 2              | 2            |
| AmAtUrECoDeR     | 422320.57  | 24    | 03.17.2019 10:00:49   | Java      | 6              | 2            |
| mmammel          | 253987.14  | 25    | 03.17.2019 00:12:41   | C++       | 2              | 1            |
| onigiri          | 145261.74  | 26    | 03.17.2019 10:11:38   | Java      | 0              | 1            |
| abdullahkool768  | 2478.86    | 27    | 03.16.2019 04:18:04   | C++       | 2              | 1            |
| the_lizard       | 945.31     | 28    | 03.17.2019 08:57:38   | C++       | 1              | 1            |
| adist98          | 161.33     | 29    | 03.17.2019 07:10:24   | C++       | 9              | 3            |
| r2d1             |            |       |                       |           | 1              | 0            |
| sumoru           |            |       |                       |           | 8              | 0            |
| usto             |            |       |                       |           | 1              | 0            |
| yarrr            |            |       |                       |           | 1              | 0            |

</details>

1位の wleite さんと比べると点数を 981684.78 / 824296.54 = 1.1909364 倍にできることが分かる。
現状の点数は 0.14366054 * 0.62282140 であるが、辺とルートをそれぞれ √2 倍すればよいと考えると 0.15737224 * 0.68226666 が目標となる。

まだ単純な貪欲解なので10位でも悲観することはなさそう。

### 2019/03/18 03:40

forum を読むのを忘れていたので読んだ。

visualizer の更新があったので更新した。
branch [official-visualizer](https://github.com/kmyk/topcoder-marathon-match-109-road-network/tree/official-visualizer) を生やして merge してあげたら手元での更新との合成がとても楽だった。

### 2019/03/18 04:10

ひとつめのルートを総当たり `42d69838a9dac2320aabaab8afeebb6efcebbd1c` を提出。バグ修正済み。
でも9位。これはちょっと焦る。
赤維持どころか [TCO Japan regional](https://tco19.topcoder.com/regional-events/tco19-regional-event-japan) に行くにも点数が足りなさそう。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| wleite          | 981637    |      1 | 03.17.2019 13:35:50    | Java       |               6 |             6 |
| EvbCFfp1XB      | 932661    |      2 | 03.17.2019 09:24:29    | Java       |               2 |             2 |
| mugurelionut    | 921833    |      3 | 03.17.2019 11:33:56    | C++        |              13 |             4 |
| yowa            | 919671    |      4 | 03.17.2019 11:18:16    | C++        |               3 |             3 |
| iwashi31        | 910202    |      5 | 03.17.2019 10:24:25    | C++        |               9 |             6 |
| nika            | 904129    |      6 | 03.16.2019 16:19:35    | C++        |               2 |             1 |
| kotamanegi      | 891958    |      7 | 03.17.2019 04:41:56    | C++        |               4 |             3 |
| tanzaku         | 887557    |      8 | 03.17.2019 14:07:24    | C++        |               3 |             3 |
| kimiyuki        | 887552    |      9 | 03.17.2019 15:49:30    | C++        |               5 |             2 |
| iehn            | 850720    |     10 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| gasin           | 837488    |     11 | 03.16.2019 22:09:25    | C++        |               2 |             2 |
| kosakkun        | 819480    |     12 | 03.17.2019 11:34:26    | C++        |               4 |             4 |
| HrKdr           | 791097    |     13 | 03.17.2019 15:30:35    | C++        |               3 |             3 |
| windhunterSB    | 772335    |     14 | 03.17.2019 11:25:38    | C++        |               3 |             3 |
| yuya178         | 759916    |     15 | 03.17.2019 02:08:29    | Java       |              12 |             4 |
| my316g          | 709301    |     16 | 03.17.2019 10:53:07    | C++        |               1 |             1 |
| vi002           | 699248    |     17 | 03.17.2019 14:46:17    | C++        |               3 |             3 |
| ts__            | 655205    |     18 | 03.17.2019 12:32:38    | C++        |               2 |             2 |
| nel215          | 651614    |     19 | 03.17.2019 04:11:07    | C++        |               1 |             1 |
| seess_yshysj    | 568561    |     20 | 03.16.2019 21:23:06    | C++        |               8 |             3 |
| kishore_g84     | 523461    |     21 | 03.17.2019 02:23:53    | Java       |               1 |             2 |
| ebicochineal    | 477425    |     22 | 03.17.2019 05:51:20    | C++        |               1 |             3 |
| bduvenhage      | 459641    |     23 | 03.17.2019 02:52:33    | C++        |               2 |             2 |
| AmAtUrECoDeR    | 422303    |     24 | 03.17.2019 10:00:49    | Java       |               6 |             2 |
| mmammel         | 253978    |     25 | 03.17.2019 00:12:41    | C++        |               2 |             1 |
| onigiri         | 145254    |     26 | 03.17.2019 10:11:38    | Java       |               0 |             1 |
| abdullahkool768 |   2478.86 |     27 | 03.16.2019 04:18:04    | C++        |               2 |             1 |
| the_lizard      |    944.96 |     28 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| adist98         |    161.32 |     29 | 03.17.2019 07:10:24    | C++        |               9 |             3 |
| r2d1            |           |        |                        |            |               1 |             0 |
| sumoru          |           |        |                        |            |               8 |             0 |
| usto            |           |        |                        |            |               1 |             0 |
| yarrr           |           |        |                        |            |               1 |             0 |

</details>

ところで `oj submit` の `--full-submission` が壊れた？
さっきは提出に成功したのに今は → デグレ <https://github.com/kmyk/online-judge-tools/issues/393>

ちなみに点数はちゃんと上がってる。

``` console
$ for commit in 42d69838a9dac2320aabaab8afeebb6efcebbd1c ; do for i in $(seq 2000) ; do cat log/${commit}/${i}.json ; done | jq --slurp '[ ([ .[] | .details.edgeScore / ([ .edges[] | .[3] ] | add) ] | add / length), ([ .[] | .details.routeScore / ([ .routes[] | .[2] ] | add) ] | add / length) ]' ; done
[
  0.1466205469750911,
  0.6530273896351924
]
```

### 2019/03/18 06:30

使用する辺を追加や交換する単純な焼きなましを書くべきな気がしてきた。

解法本体と visualizer や Makefile などを別の git の commit tree で管理したい気持ちがあるが、実はできるらしい: [Two git repositories in one directory? - Stack Overflow](https://stackoverflow.com/questions/436125/two-git-repositories-in-one-directory), [Two different Git repo in same directory - Stack Overflow](https://stackoverflow.com/questions/8778796/two-different-git-repo-in-same-directory)。
しかし微妙に面倒そうでもある。
現状では `documents` ディレクトリ内に [documents](https://github.com/kmyk/topcoder-marathon-match-109-road-network/tree/documents) branch を clone してをそこを別管理としているが、 `tools` のようなディレクトリを切って同様のことをする方が楽そうである。


### 2019/03/18 08:20

使用するルートの集合を固定されたとき、その充足可能性判定はできるか？ あるいはその費用の最小値は求まるか？

グラフは平面グラフに限られているように見えるが、その事実は使えないか？

ルートとしてありそうなパスをいくつか列挙し、これの部分集合を焼きなますのはどうか？

### 2019/03/18 09:20

visualizer を読んだ。
平面グラフを作ってる。

1.  頂点を座標付きでランダムに生成する
1.  全体が連結になるまで辺を次のように足す:
    -   点 a をランダムに選ぶ
    -   その最も近くてかつ平面描画であることを崩さない点 b を選ぶ
    -   費用 m は距離 d(a, b) をそのまま使う
    -   価値 p は距離 d(a, b) を係数としてランダムに選ぶ
1.  ルートをランダムに生成する
    -   価値 p は距離 d(a, b) をそのまま使う
1.  予算 NM はルートの価値の総和を係数としてランダムに選ぶ

### 2019/03/18 09:50

閉路をひとつ選んでその中の辺の使用/不使用を反転させるのはどうか？
特に長さ 3 の閉路ですべてが使用でもすべてが不使用でもないものを考えると、これは近傍の内径をかなり小さくできそう。

### 2019/03/18 10:40

画像を出力できるようにした

<details>
<summary>画像: 貪欲, 辺コスパ順</summary>

[6c0b6a595739e994b57aeb153393bd0a933445f9](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/6c0b6a595739e994b57aeb153393bd0a933445f9)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/6c0b6a595739e994b57aeb153393bd0a933445f9.10.png)

</details>

<details>
<summary>画像: 貪欲, ルートコスパ順</summary>

[20673a651715e1226363c8910012077444081827](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/20673a651715e1226363c8910012077444081827)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/20673a651715e1226363c8910012077444081827.10.png)

</details>

<details>
<summary>画像: 貪欲, ルートコスパ順 + 1手目総当たり</summary>

[42d69838a9dac2320aabaab8afeebb6efcebbd1c](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/42d69838a9dac2320aabaab8afeebb6efcebbd1c)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/42d69838a9dac2320aabaab8afeebb6efcebbd1c.10.png)

</details>

<details>
<summary>画像: 焼きなまし (バグ)</summary>

[24418d448d1734bc19dc2055b3791f28a682972c](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/24418d448d1734bc19dc2055b3791f28a682972c)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/beac3be54a4246ae8c4c11edbe5c8ca2b080b31f/images/24418d448d1734bc19dc2055b3791f28a682972c.10.png)

</details>

### 2019/03/18 11:20

与えられた辺集合から、達成できたルートを崩さないように辺を最小化できるか？
それはどのような目的関数に対するもので、どのような計算量のものか？

### 2019/03/18 11:45

平面グラフを復元し、その上で十字やY字の形になるように初期状態を決めてやるのはどうか。
あるルートの充足のためのパスの選択をする際に、左上に寄せる / 右下に寄せる などとして座標情報を用いれば多様性が確保できないか。

### 2019/03/18 22:50

雑な焼きなまし

``` console
$ for commit in 24418d448d1734bc19dc2055b3791f28a682972c ; do for i in $(seq 2000) ; do cat log/${commit}/${i}.json ; done | jq --slurp '[ ([ .[] | .details.edgeScore / ([ .edges[] | .[3] ] | add) ] | add / length), ([ .[] | .details.routeScore / ([ .routes[] | .[2] ] | add) ] | add / length) ]' ; done
[
  0.15678730519465703,
  0.48109973942333095
]
```

### 2019/03/19 13:00

なにやら rejudge があったらしい。
過去の考察に影響はなさそう。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| EvbCFfp1XB      | 985146    |      1 | 03.18.2019 21:04:12    | Java       |               5 |             5 |
| mugurelionut    | 967615    |      2 | 03.18.2019 23:01:41    | C++        |              19 |             7 |
| wleite          | 963118    |      3 | 03.17.2019 22:03:42    | Java       |               8 |             8 |
| kosakkun        | 920489    |      4 | 03.18.2019 21:11:44    | C++        |               9 |             7 |
| Daiver19        | 920466    |      5 | 03.18.2019 23:44:26    | C++        |               5 |             2 |
| kotamanegi      | 908637    |      6 | 03.18.2019 04:06:17    | C++        |               7 |             4 |
| yowa            | 900900    |      7 | 03.17.2019 11:18:16    | C++        |               3 |             3 |
| iwashi31        | 891421    |      8 | 03.17.2019 10:24:25    | C++        |               9 |             6 |
| nika            | 885310    |      9 | 03.16.2019 16:19:35    | C++        |               2 |             1 |
| ebicochineal    | 878139    |     10 | 03.18.2019 23:10:41    | C++        |               4 |             6 |
| kimiyuki        | 869105    |     11 | 03.17.2019 15:49:30    | C++        |               6 |             2 |
| tanzaku         | 869001    |     12 | 03.18.2019 20:26:32    | C++        |               5 |             4 |
| ts__            | 861328    |     13 | 03.18.2019 23:00:31    | C++        |               3 |             3 |
| iehn            | 833566    |     14 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| HrKdr           | 830086    |     15 | 03.18.2019 20:19:52    | C++        |               4 |             4 |
| yuya178         | 823077    |     16 | 03.17.2019 22:50:41    | Java       |              14 |             5 |
| my316g          | 820482    |     17 | 03.18.2019 20:54:03    | C++        |               3 |             3 |
| gasin           | 820437    |     18 | 03.16.2019 22:09:25    | C++        |               2 |             2 |
| kurenai3110     | 760963    |     19 | 03.17.2019 20:15:43    | C++        |               1 |             1 |
| kishore_g84     | 758747    |     20 | 03.17.2019 17:46:38    | Java       |               2 |             3 |
| windhunterSB    | 757266    |     21 | 03.17.2019 11:25:38    | C++        |               3 |             3 |
| vi002           | 685226    |     22 | 03.17.2019 14:46:17    | C++        |               3 |             3 |
| nel215          | 640725    |     23 | 03.17.2019 04:11:07    | C++        |               1 |             1 |
| seess_yshysj    | 623207    |     24 | 03.18.2019 21:00:42    | C++        |              12 |             5 |
| mmammel         | 559216    |     25 | 03.18.2019 21:10:25    | C++        |               3 |             2 |
| sullyper        | 520073    |     26 | 03.18.2019 05:19:33    | C++        |               1 |             1 |
| bduvenhage      | 502708    |     27 | 03.18.2019 01:19:51    | C++        |               3 |             3 |
| AmAtUrECoDeR    | 414478    |     28 | 03.17.2019 10:00:49    | Java       |              10 |             2 |
| wadissimo       | 348696    |     29 | 03.17.2019 16:11:18    | Java       |               1 |             1 |
| tuff            | 284590    |     30 | 03.18.2019 03:31:32    | C++        |               0 |             1 |
| onigiri         | 182242    |     31 | 03.18.2019 20:06:07    | Java       |               0 |             3 |
| mcw1142         |  83062.4  |     32 | 03.18.2019 23:08:52    | Java       |               3 |             2 |
| fmhr            |  12925.2  |     33 | 03.18.2019 04:34:16    | C++        |               0 |             2 |
| neetsdkasu      |   5693.69 |     34 | 03.17.2019 17:46:18    | VB         |               1 |             1 |
| abdullahkool768 |   2465.82 |     35 | 03.16.2019 04:18:04    | C++        |               2 |             1 |
| adist98         |   2465.82 |     35 | 03.17.2019 21:20:42    | C++        |              12 |             4 |
| DebNatkh        |   2465.82 |     35 | 03.17.2019 17:57:30    | C++        |               0 |             1 |
| the_lizard      |    918.85 |     38 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| id              |      0    |     39 | 03.18.2019 14:27:32    | C#         |               1 |             1 |
| ArtDitel        |           |        |                        |            |               2 |             0 |
| Aruru           |           |        |                        |            |               1 |             0 |
| r2d1            |           |        |                        |            |               1 |             0 |
| sumoru          |           |        |                        |            |              10 |             0 |
| tourist         |           |        |                        |            |               1 |             0 |
| usto            |           |        |                        |            |               1 |             0 |
| yarrr           |           |        |                        |            |               1 |             0 |

</details>

### 2019/03/20 19:00

ルートごとに良さそうなパスを列挙してこれを焼き鈍してみた。
はずれ。

複数のルートのパスを重ねないといけないのにこれを取り扱えないことが失敗の原因。
となると、現在の連結部分からの各点への良いパスをそれぞれ持っておいて2点選べばいい感じになる？
どう削除するかが難しさだから上手い追加を考えても無駄？

### 2019/03/20 19:30

焼き鈍し解にバグを見つけたので修正。
`0e16762cd24f558cc6afb05f39852b92d58aea14`。

### 2019/03/20 20:30

バグ修正後の焼き鈍し解を提出するも悪化。
26位。元は18位ぐらい。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| gasin           | 985782    |      1 | 03.20.2019 07:18:05    | C++        |               8 |             8 |
| EvbCFfp1XB      | 972870    |      2 | 03.19.2019 09:49:09    | Java       |               6 |             6 |
| wleite          | 968413    |      3 | 03.20.2019 03:10:43    | Java       |              12 |            12 |
| tourist         | 966358    |      4 | 03.19.2019 17:39:50    | C++        |               5 |             3 |
| nika            | 952828    |      5 | 03.20.2019 05:44:51    | C++        |               4 |             4 |
| mugurelionut    | 948779    |      6 | 03.18.2019 23:01:41    | C++        |              20 |             7 |
| kosakkun        | 928734    |      7 | 03.20.2019 02:13:33    | C++        |              21 |            13 |
| kotamanegi      | 926787    |      8 | 03.20.2019 06:10:51    | C++        |              15 |             8 |
| onigiri         | 917791    |      9 | 03.19.2019 08:32:49    | Java       |               2 |             5 |
| tanzaku         | 910265    |     10 | 03.19.2019 13:13:05    | C++        |               8 |             6 |
| Daiver19        | 902715    |     11 | 03.18.2019 23:44:26    | C++        |               5 |             2 |
| iwashi31        | 901386    |     12 | 03.19.2019 08:54:49    | C++        |              10 |             7 |
| Stefan70        | 897016    |     13 | 03.19.2019 18:17:02    | Java       |               3 |             3 |
| ebicochineal    | 888628    |     14 | 03.20.2019 01:14:50    | C++        |               4 |             9 |
| yowa            | 883509    |     15 | 03.17.2019 11:18:16    | C++        |               3 |             3 |
| __math          | 869100    |     16 | 03.19.2019 01:09:08    | C++        |               0 |             1 |
| ts__            | 869018    |     17 | 03.20.2019 06:31:31    | C++        |               4 |             4 |
| my316g          | 859186    |     18 | 03.19.2019 17:55:23    | C++        |               4 |             4 |
| yuya178         | 826219    |     19 | 03.20.2019 01:11:57    | Java       |              16 |             6 |
| tsukasa_diary   | 824397    |     20 | 03.20.2019 03:33:20    | C++        |               4 |             3 |
| iehn            | 817875    |     21 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| HrKdr           | 814177    |     22 | 03.19.2019 13:56:57    | C++        |               4 |             5 |
| kozima          | 802078    |     23 | 03.19.2019 19:33:15    | C++        |               3 |             2 |
| sumoru          | 800078    |     24 | 03.19.2019 11:11:15    | C++        |              11 |             1 |
| AmAtUrECoDeR    | 783714    |     25 | 03.19.2019 00:20:27    | Java       |              12 |             3 |
| kimiyuki        | 770853    |     26 | 03.20.2019 06:17:48    | C++        |               7 |             3 |
| okazaki         | 761313    |     27 | 03.19.2019 23:44:50    | Java       |               0 |             3 |
| kishore_g84     | 756933    |     28 | 03.19.2019 01:55:44    | Java       |               3 |             4 |
| kurenai3110     | 746505    |     29 | 03.17.2019 20:15:43    | C++        |               1 |             1 |
| windhunterSB    | 742587    |     30 | 03.17.2019 11:25:38    | C++        |               3 |             3 |
| H_a_s_h         | 742457    |     31 | 03.19.2019 17:44:24    | C++        |               2 |             2 |
| vi002           | 742147    |     32 | 03.19.2019 04:06:21    | C++        |               4 |             4 |
| Aruru           | 722875    |     33 | 03.19.2019 17:08:16    | C++        |               3 |             2 |
| kruntuid        | 685429    |     34 | 03.20.2019 03:26:45    | C++        |               1 |             2 |
| ArtDitel        | 671095    |     35 | 03.19.2019 08:03:19    | C++        |               3 |             1 |
| fetetriste      | 657602    |     36 | 03.19.2019 16:19:27    | Java       |               1 |             1 |
| abdullahkool768 | 630805    |     37 | 03.19.2019 01:26:43    | C++        |               3 |             2 |
| seess_yshysj    | 611846    |     38 | 03.19.2019 08:05:24    | C++        |              14 |             8 |
| phyllo          | 605040    |     39 | 03.19.2019 12:23:56    | C++        |               1 |             1 |
| nel215          | 603996    |     40 | 03.19.2019 18:16:17    | C++        |               1 |             3 |
| mmammel         | 589173    |     41 | 03.19.2019 15:47:41    | C++        |               5 |             4 |
| sullyper        | 512965    |     42 | 03.18.2019 05:19:33    | C++        |               1 |             1 |
| bduvenhage      | 494009    |     43 | 03.18.2019 01:19:51    | C++        |               3 |             3 |
| nii_nii         | 472960    |     44 | 03.19.2019 09:38:25    | C++        |               7 |             3 |
| KShiv           | 392066    |     45 | 03.19.2019 11:30:33    | Java       |               2 |             1 |
| wadissimo       | 342443    |     46 | 03.17.2019 16:11:18    | Java       |               1 |             1 |
| tuff            | 279755    |     47 | 03.18.2019 03:31:32    | C++        |               0 |             1 |
| adist98         | 143356    |     48 | 03.20.2019 06:51:07    | C++        |              19 |             8 |
| mcw1142         |  85937.9  |     49 | 03.19.2019 20:20:26    | Java       |               4 |             3 |
| wvsever         |  65981.2  |     50 | 03.20.2019 03:51:56    | C++        |               5 |             1 |
| fmhr            |  12844.2  |     51 | 03.18.2019 04:34:16    | C++        |               0 |             2 |
| neetsdkasu      |   5621.34 |     52 | 03.17.2019 17:46:18    | VB         |               1 |             1 |
| olphe           |   3727.85 |     53 | 03.19.2019 14:18:52    | C++        |              10 |             1 |
| DebNatkh        |   2461.23 |     54 | 03.17.2019 17:57:30    | C++        |               0 |             1 |
| the_lizard      |    901.77 |     55 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| id              |      0    |     56 | 03.18.2019 14:27:32    | C#         |               1 |             1 |
| r2d1            |           |        |                        |            |               1 |             0 |
| roto37          |           |        |                        |            |               1 |             0 |
| usto            |           |        |                        |            |               1 |             0 |
| yarrr           |           |        |                        |            |               1 |             0 |

</details>

### 2019/03/21 03:40

初手で書いた貪欲に勝てない。
ずっとやってるのに、なぜ。

特に、そもそもの焼きなましはこの貪欲ベースで行なっている。
1 回 60 msec ほどの貪欲に 1.16 倍ほど勝てないのはおかしい。
1 位までさらに 1.15 倍ほどの差があるのも分からない。

使用するルートの集合を固定されたときの充足可能性判定について。
ルートの集合が 1 点のみなら、初めに全点対間最短距離を求めておくことで、解の辺集合 X が連結と仮定すれば O(|X|) で可能。
解が与えられてそこからの最小化は、グラフ中の三角形や四角形を列挙し、解の各点の次数を覚えておくことで O(|X|) の貪欲な近似がある。

解は基本的に木か森になるはず。
最小全域木を持っておくとうまく使えないだろうか？

### 2019/03/21 08:00

最小全域木の方向を思い返したら Steiner 最小木問題がまさにこれだった。
iz が ICPC ライブラリに入れてたけど一度も使わなかった思い出の木である。
名前は思い出していたが Euclid 空間の話に限ると思っていたので考察の枝を刈ってしまっていた。

-   [シュタイナー最小木 - ORWiki](http://www.orsj.or.jp/~wiki/wiki/index.php/%E3%82%B7%E3%83%A5%E3%82%BF%E3%82%A4%E3%83%8A%E3%83%BC%E6%9C%80%E5%B0%8F%E6%9C%A8)
-   浅野孝夫, et al. シュタイナー森問題に対する近似アルゴリズムの性能評価. 研究報告アルゴリズム (AL), 2016, 2016.10: 1-8. [pdf](https://ipsj.ixsq.nii.ac.jp/ej/?action=repository_action_common_download&item_id=161674&item_no=1&attribute_id=1&file_no=1)
-   [Spaghetti Source - 最小シュタイナー木](http://www.prefield.com/algorithm/dp/steiner_tree.html)

NP 困難であり、平面グラフに制限してもなお NP 困難らしい。
貪欲近似解としては終端点を結ぶ最良のパスを貪欲に縮約していくものがある。
線形計画問題として定式化して線形緩和な双対問題を解く近似解が有名らしい。

### 2019/03/21 13:20

実装をしていく

### 2019/03/21 18:40

できた。
ルートの集合を焼き鈍し、これを固定して P を無視して M だけで最小シュタイナー森問題として貪欲な併合で解くと `41343d3608166b6e6692ee3c570e2a99391638e6`。
P を考慮に入れるための調整をして `655f2e2886e0fcbeb4d315e17915079c187914f6` はいくつかの問題で元の貪欲に勝つ。
しかし1回の貪欲に焼きなまし全体で勝ちうるというだけで、多数回の貪欲にはまだ勝てていない。

ルート追加の貪欲を今回の併合を使って改善できないか。
あるいはそろそろビームサーチ化するべきではないか。

### 2019/03/21 19:00

影響範囲の大きいバグを見つけてしまった `8cf9e87678c74ef3d8dac4a77eec83509047d639`。
修正をしたら多数回の貪欲と同じぐらいになった。
他の考察の再検証も必要。

### 2019/03/21 22:00

ルートの集合を貪欲に増やして評価関数は貪欲の併合を書いた。
ぜんぜんだめ。
なぜ？

三角形や四角形からひとつ辺が落ちた部分グラフを見付けていい感じに回転させるものを書いた。
すこしは良くなるようだが速度も遅いので確認してから。

### 2019/03/21 22:10

`8cf9e87678c74ef3d8dac4a77eec83509047d639` を提出。
21位と渋い。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| gasin           | 985254    |      1 | 03.20.2019 17:52:31    | C++        |               9 |             9 |
| EvbCFfp1XB      | 972667    |      2 | 03.19.2019 09:49:09    | Java       |               6 |             6 |
| wleite          | 968225    |      3 | 03.20.2019 03:10:43    | Java       |              12 |            12 |
| tourist         | 966990    |      4 | 03.20.2019 20:48:42    | C++        |               6 |             4 |
| nika            | 956859    |      5 | 03.20.2019 14:06:35    | C++        |               4 |             5 |
| mugurelionut    | 954514    |      6 | 03.20.2019 18:31:49    | C++        |              23 |            10 |
| tanzaku         | 952558    |      7 | 03.20.2019 21:35:16    | C++        |               9 |             8 |
| iwashi31        | 929027    |      8 | 03.20.2019 09:40:03    | C++        |              10 |             8 |
| kosakkun        | 924682    |      9 | 03.21.2019 08:40:34    | C++        |              27 |            16 |
| kotamanegi      | 924620    |     10 | 03.21.2019 07:58:55    | C++        |              18 |            11 |
| sumoru          | 920228    |     11 | 03.21.2019 08:02:46    | C++        |              14 |             4 |
| onigiri         | 917621    |     12 | 03.19.2019 08:32:49    | Java       |               2 |             5 |
| ebicochineal    | 909499    |     13 | 03.20.2019 13:26:16    | C++        |               4 |            11 |
| __math          | 905615    |     14 | 03.21.2019 05:26:46    | C++        |               0 |             2 |
| yowa            | 901101    |     15 | 03.21.2019 05:51:43    | C++        |               5 |             5 |
| Stefan70        | 896855    |     16 | 03.19.2019 18:17:02    | Java       |               3 |             3 |
| Daiver19        | 894389    |     17 | 03.21.2019 01:40:16    | C++        |               9 |             3 |
| tsukasa_diary   | 880640    |     18 | 03.20.2019 08:28:03    | C++        |               5 |             4 |
| ts__            | 868893    |     19 | 03.20.2019 06:31:31    | C++        |               4 |             4 |
| my316g          | 859037    |     20 | 03.19.2019 17:55:23    | C++        |               4 |             4 |
| kimiyuki        | 858548    |     21 | 03.21.2019 09:06:20    | C++        |              10 |             4 |
| nel215          | 853167    |     22 | 03.21.2019 03:30:34    | C++        |               1 |             5 |
| phyllo          | 839925    |     23 | 03.21.2019 05:41:00    | C++        |               5 |             5 |
| okazaki         | 837538    |     24 | 03.20.2019 23:30:25    | Java       |               0 |             5 |
| kozima          | 835447    |     25 | 03.20.2019 12:13:18    | C++        |               5 |             4 |
| yuya178         | 831888    |     26 | 03.21.2019 02:29:44    | Java       |              18 |            10 |
| AmAtUrECoDeR    | 823285    |     27 | 03.20.2019 22:30:24    | Java       |              15 |             4 |
| olphe           | 819127    |     28 | 03.20.2019 16:04:05    | C++        |              27 |             3 |
| iehn            | 817721    |     29 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| HrKdr           | 813985    |     30 | 03.19.2019 13:56:57    | C++        |               4 |             5 |
| kishore_g84     | 785182    |     31 | 03.21.2019 02:01:50    | Java       |               3 |             6 |
| tashikani       | 780218    |     32 | 03.21.2019 08:14:31    | C++        |               5 |             3 |
| windhunterSB    | 778669    |     33 | 03.20.2019 11:06:30    | C++        |               4 |             4 |
| Aruru           | 748391    |     34 | 03.20.2019 08:00:45    | C++        |               4 |             3 |
| kurenai3110     | 746381    |     35 | 03.17.2019 20:15:43    | C++        |               1 |             1 |
| arukuka         | 744557    |     36 | 03.21.2019 07:47:31    | C++        |               4 |             1 |
| H_a_s_h         | 742346    |     37 | 03.19.2019 17:44:24    | C++        |               2 |             2 |
| vi002           | 741999    |     38 | 03.19.2019 04:06:21    | C++        |               4 |             4 |
| ArtDitel        | 671009    |     39 | 03.19.2019 08:03:19    | C++        |               3 |             1 |
| fetetriste      | 657470    |     40 | 03.19.2019 16:19:27    | Java       |               1 |             1 |
| sullyper        | 650033    |     41 | 03.20.2019 12:01:27    | C++        |               2 |             2 |
| abdullahkool768 | 630750    |     42 | 03.19.2019 01:26:43    | C++        |               3 |             2 |
| seess_yshysj    | 611688    |     43 | 03.19.2019 08:05:24    | C++        |              14 |             8 |
| kruntuid        | 607393    |     44 | 03.21.2019 03:03:14    | C++        |               2 |             4 |
| _sash           | 599518    |     45 | 03.20.2019 22:36:07    | C++        |               1 |             2 |
| mmammel         | 589035    |     46 | 03.19.2019 15:47:41    | C++        |               5 |             4 |
| kwrig           | 527494    |     47 | 03.21.2019 08:22:31    | Java       |               3 |             3 |
| mcw1142         | 521979    |     48 | 03.20.2019 20:27:03    | Java       |               7 |             5 |
| bduvenhage      | 512070    |     49 | 03.21.2019 05:14:58    | C++        |               5 |             4 |
| nii_nii         | 472868    |     50 | 03.19.2019 09:38:25    | C++        |               7 |             3 |
| KShiv           | 471384    |     51 | 03.21.2019 07:41:53    | Java       |               3 |             2 |
| wvsever         | 466854    |     52 | 03.21.2019 06:57:26    | C++        |               6 |             2 |
| ty70            | 438706    |     53 | 03.21.2019 04:24:20    | C++        |               1 |             1 |
| nagyt           | 388689    |     54 | 03.20.2019 18:01:33    | C++        |               1 |             1 |
| WhiteParadise   | 362562    |     55 | 03.20.2019 16:22:57    | Java       |               2 |             2 |
| wadissimo       | 342392    |     56 | 03.17.2019 16:11:18    | Java       |               1 |             1 |
| tuff            | 279707    |     57 | 03.18.2019 03:31:32    | C++        |               0 |             1 |
| roto37          | 275836    |     58 | 03.21.2019 03:32:12    | C++        |               2 |             1 |
| adist98         | 204780    |     59 | 03.21.2019 05:22:34    | C++        |              34 |            14 |
| fmhr            |  12842.1  |     60 | 03.18.2019 04:34:16    | C++        |               0 |             2 |
| neetsdkasu      |   5620.29 |     61 | 03.17.2019 17:46:18    | VB         |               1 |             1 |
| DebNatkh        |   2461.16 |     62 | 03.17.2019 17:57:30    | C++        |               0 |             1 |
| TomA-_-         |   2461.16 |     62 | 03.20.2019 11:05:59    | C#         |               7 |             1 |
| the_lizard      |    901.91 |     64 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| id              |      0    |     65 | 03.18.2019 14:27:32    | C#         |               1 |             1 |
| r2d1            |           |        |                        |            |               1 |             0 |
| usto            |           |        |                        |            |               3 |             0 |
| yarrr           |           |        |                        |            |               1 |             0 |

</details>

### 2019/03/22 21:00

貪欲の評価関数を変化させて結果を見てみた。
seed = 2 で次のようになった。
最初が一番良いの分からない。

``` c++
double value = (double)(sum_p[component_of[route.b]] + route.p) / sum_m[component_of[route.b]];  // 1141 819
double value = route.p;  // 968 743
double value = sum_p[component_of[route.b]];  // 488 832
double value = - sum_m[component_of[route.b]];  // 984 774
double value = sum_p[component_of[route.b]] + route.p - sum_m[component_of[route.b]];  // 856 826
double value = (double)sum_p[component_of[route.b]] / sum_m[component_of[route.b]];  // 740 950
double value = (double)route.p / sum_m[component_of[route.b]];  // 1170 746
double value = (double)(sum_p[component_of[route.b]] + 2 * route.p) / sum_m[component_of[route.b]];  // 1187 792
double value = (double)(sum_p[component_of[route.b]] + 0.5 * route.p) / sum_m[component_of[route.b]];  // 1120 839
double value = (double)(sum_p[component_of[route.b]] + route.p) / (10 + sum_m[component_of[route.b]]);  // 906 854
double value = (double)(foo * sum_p[component_of[route.b]] + bar * route.p) / sum_m[component_of[route.b]];  // 847 942  (foo is sum_p of routes, bar is sum_p of edges)
```

### 2019/03/23 00:00

何も分からない。
他のやるべき事の進捗もまずいし、とても良くない。

平面描画の復元は、天才ぽいけど筋が悪そうなのでまだ保留したい。
しかし保留の理由に「面倒なので」も少なからず存在するのはよくない。

貪欲の結果のルート集合を貪欲併合に食わせてみた。
点数はほぼそのままに費用だけ減少した。
seed = 2 で費用 252 -> 240 という結果であった。
ここから、ルート集合の構築や焼き鈍しがへただと言うことが分かる。

### 2019/03/23 02:10

ルート集合を焼きなますとき、予期せず完成したルートも集合に含めて更新することにした。
これにより「なぜその近傍が成功だったのか」の情報が次の近傍選択に伝播する。

提出。
しかし19位と、まだスタートラインには立てていない。
`a095cda41bab002963479da57c35f3720141e931`。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| gasin           | 986515    |      1 | 03.22.2019 11:32:40    | C++        |              11 |            11 |
| wleite          | 985268    |      2 | 03.22.2019 11:30:50    | Java       |              19 |            18 |
| EvbCFfp1XB      | 973069    |      3 | 03.19.2019 09:49:09    | Java       |               6 |             6 |
| mugurelionut    | 967084    |      4 | 03.22.2019 10:53:43    | C++        |              24 |            11 |
| nika            | 965811    |      5 | 03.22.2019 03:19:11    | C++        |               5 |             6 |
| tourist         | 963912    |      6 | 03.21.2019 19:26:09    | C++        |               7 |             5 |
| tanzaku         | 942213    |      7 | 03.22.2019 12:06:37    | C++        |              11 |            12 |
| onigiri         | 933647    |      8 | 03.21.2019 16:04:32    | Java       |               3 |             7 |
| iwashi31        | 929426    |      9 | 03.20.2019 09:40:03    | C++        |              10 |             8 |
| ts__            | 929097    |     10 | 03.22.2019 10:03:35    | C++        |               6 |             6 |
| ebicochineal    | 928977    |     11 | 03.22.2019 12:42:48    | C++        |               4 |            15 |
| kosakkun        | 926664    |     12 | 03.21.2019 10:45:47    | C++        |              27 |            17 |
| yowa            | 921637    |     13 | 03.21.2019 21:59:18    | C++        |               6 |             6 |
| Daiver19        | 921549    |     14 | 03.21.2019 13:26:24    | C++        |              11 |             5 |
| sumoru          | 918250    |     15 | 03.21.2019 11:25:59    | C++        |              15 |             5 |
| kotamanegi      | 916213    |     16 | 03.22.2019 10:44:32    | C++        |              18 |            12 |
| __math          | 906008    |     17 | 03.21.2019 05:26:46    | C++        |               0 |             2 |
| Stefan70        | 897227    |     18 | 03.19.2019 18:17:02    | Java       |               3 |             3 |
| kimiyuki        | 888485    |     19 | 03.22.2019 13:04:38    | C++        |              11 |             5 |
| tsukasa_diary   | 885167    |     20 | 03.22.2019 10:11:47    | C++        |               8 |             6 |
| phyllo          | 879398    |     21 | 03.21.2019 11:11:54    | C++        |               6 |             6 |
| RafbillFr       | 876756    |     22 | 03.22.2019 11:05:41    | C++        |               2 |             2 |
| kurenai3110     | 875942    |     23 | 03.21.2019 20:30:40    | C++        |               3 |             3 |
| my316g          | 859303    |     24 | 03.19.2019 17:55:23    | C++        |               4 |             4 |
| nel215          | 853551    |     25 | 03.22.2019 11:29:24    | C++        |               1 |             6 |
| olphe           | 850958    |     26 | 03.22.2019 13:06:39    | C++        |              44 |            10 |
| tashikani       | 848437    |     27 | 03.22.2019 11:11:49    | C++        |               6 |             4 |
| okazaki         | 841888    |     28 | 03.22.2019 10:37:27    | Java       |               0 |             7 |
| kozima          | 835788    |     29 | 03.20.2019 12:13:18    | C++        |               5 |             4 |
| yuya178         | 832309    |     30 | 03.21.2019 02:29:44    | Java       |              18 |            10 |
| AmAtUrECoDeR    | 823662    |     31 | 03.20.2019 22:30:24    | Java       |              15 |             4 |
| iehn            | 818017    |     32 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| HrKdr           | 815718    |     33 | 03.22.2019 12:48:18    | C++        |               5 |             6 |
| arukuka         | 813975    |     34 | 03.22.2019 09:46:55    | C++        |               5 |             2 |
| roto37          | 807543    |     35 | 03.22.2019 03:50:23    | C++        |               4 |             3 |
| windhunterSB    | 806592    |     36 | 03.21.2019 19:12:59    | C++        |               5 |             5 |
| Aruru           | 801477    |     37 | 03.22.2019 06:34:38    | C++        |               4 |             4 |
| kishore_g84     | 785493    |     38 | 03.21.2019 02:01:50    | Java       |               3 |             6 |
| _sash           | 752360    |     39 | 03.21.2019 10:10:01    | C++        |               2 |             3 |
| H_a_s_h         | 742662    |     40 | 03.19.2019 17:44:24    | C++        |               2 |             2 |
| vi002           | 742276    |     41 | 03.19.2019 04:06:21    | C++        |               4 |             4 |
| sullyper        | 693148    |     42 | 03.22.2019 02:53:32    | C++        |               3 |             3 |
| ArtDitel        | 671311    |     43 | 03.19.2019 08:03:19    | C++        |               3 |             1 |
| KShiv           | 664890    |     44 | 03.22.2019 06:19:57    | Java       |               4 |             3 |
| fetetriste      | 657672    |     45 | 03.19.2019 16:19:27    | Java       |               1 |             1 |
| abdullahkool768 | 631184    |     46 | 03.19.2019 01:26:43    | C++        |               3 |             2 |
| bduvenhage      | 616277    |     47 | 03.22.2019 09:26:30    | C++        |               7 |             5 |
| seess_yshysj    | 611938    |     48 | 03.19.2019 08:05:24    | C++        |              14 |             8 |
| kruntuid        | 607551    |     49 | 03.21.2019 03:03:14    | C++        |               2 |             4 |
| mmammel         | 589222    |     50 | 03.19.2019 15:47:41    | C++        |               5 |             4 |
| mcw1142         | 565008    |     51 | 03.21.2019 21:19:26    | Java       |               7 |             6 |
| kwrig           | 527646    |     52 | 03.21.2019 08:22:31    | Java       |               3 |             3 |
| wvsever         | 492029    |     53 | 03.21.2019 12:45:25    | C++        |               7 |             3 |
| nii_nii         | 472941    |     54 | 03.19.2019 09:38:25    | C++        |               7 |             3 |
| ty70            | 444125    |     55 | 03.22.2019 04:07:46    | C++        |               2 |             2 |
| nagyt           | 388814    |     56 | 03.20.2019 18:01:33    | C++        |               1 |             1 |
| WhiteParadise   | 362713    |     57 | 03.20.2019 16:22:57    | Java       |               2 |             2 |
| wadissimo       | 342518    |     58 | 03.17.2019 16:11:18    | Java       |               1 |             1 |
| tuff            | 279818    |     59 | 03.18.2019 03:31:32    | C++        |               0 |             1 |
| usto            | 235116    |     60 | 03.21.2019 11:58:15    | Python     |               6 |             1 |
| adist98         | 204800    |     61 | 03.21.2019 05:22:34    | C++        |              34 |            14 |
| ValGrowth       | 188833    |     62 | 03.22.2019 11:58:17    | C++        |               1 |             1 |
| TomA-_-         | 172993    |     63 | 03.22.2019 08:20:18    | C#         |              18 |             3 |
| seibu_md_4562   | 145264    |     64 | 03.21.2019 19:54:44    | C#         |               3 |             1 |
| neetsdkasu      | 106993    |     65 | 03.21.2019 13:11:03    | VB         |               2 |             2 |
| fmhr            |  12839.5  |     66 | 03.18.2019 04:34:16    | C++        |               0 |             2 |
| DebNatkh        |   2461.3  |     67 | 03.17.2019 17:57:30    | C++        |               0 |             1 |
| the_lizard      |    901.37 |     68 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| Hari_Om_Pandey  |      0    |     69 | 03.21.2019 15:57:07    | Python     |               2 |             2 |
| id              |      0    |     69 | 03.18.2019 14:27:32    | C#         |               1 |             1 |
| r2d1            |           |        |                        |            |               1 |             0 |
| tobari          |           |        |                        |            |               1 |             0 |
| yarrr           |           |        |                        |            |               1 |             0 |

</details>

### 2019/03/24 17:40

seed = 1 ~ 10 の辺スコアの最大値を見た。

-   120
-   1260
-   1895
-   220
-   675
-   305
-   75
-   300
-   290
-   285

1100 * 800 を 1000 * 900 にすると点数は増えるが 880000 から 900000 なので弱い。
総和を保つような変化は考えなくてよさそう。

### 2019/03/24 18:00

焼きなましを高速化しても改善には繋がらない。

P の大きい辺やルートを優先的に選ぶように？

### 2019/03/25 01:00

正解を引いた。
Warshall-Floyd法での前計算の部分での辺選択を、 M の値をまったく無視して r = P / M や (6 - r) M などにすると改善した。
焼きなましを高速化しても改善には繋がらないことなどから消去法でこの部分を疑えたのが勝因ぽい。

提出すると 14 位とぎりぎり赤ぱふぉ。
`473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376`。

<details>
<summary>順位表</summary>

| Handle          |     Score |   Rank | Last Submission Time   | Language   |   Example Tests |   Submissions |
|-----------------|-----------|--------|------------------------|------------|-----------------|---------------|
| gasin           | 987560    |      1 | 03.24.2019 10:20:35    | C++        |              15 |            15 |
| wleite          | 985041    |      2 | 03.23.2019 11:28:52    | Java       |              22 |            20 |
| mugurelionut    | 981271    |      3 | 03.24.2019 09:29:49    | C++        |              36 |            20 |
| EvbCFfp1XB      | 973154    |      4 | 03.23.2019 17:02:53    | Java       |               7 |             7 |
| nika            | 964893    |      5 | 03.22.2019 16:44:46    | C++        |               7 |             7 |
| tourist         | 964802    |      6 | 03.23.2019 12:34:06    | C++        |               8 |             7 |
| ts__            | 957926    |      7 | 03.24.2019 05:46:11    | C++        |               8 |             8 |
| tanzaku         | 954097    |      8 | 03.23.2019 04:06:17    | C++        |              15 |            15 |
| yowa            | 950575    |      9 | 03.23.2019 23:44:06    | C++        |               8 |             8 |
| onigiri         | 950249    |     10 | 03.24.2019 09:20:18    | Java       |              10 |            10 |
| phyllo          | 946915    |     11 | 03.24.2019 11:05:34    | C++        |              10 |            10 |
| RafbillFr       | 945582    |     12 | 03.24.2019 09:36:46    | C++        |              12 |             7 |
| ebicochineal    | 929408    |     13 | 03.24.2019 07:07:40    | C++        |               6 |            16 |
| kimiyuki        | 928399    |     14 | 03.24.2019 12:06:26    | C++        |              14 |             7 |
| iwashi31        | 927526    |     15 | 03.20.2019 09:40:03    | C++        |              10 |             8 |
| kosakkun        | 925363    |     16 | 03.23.2019 21:07:53    | C++        |              28 |            19 |
| kotamanegi      | 925055    |     17 | 03.24.2019 09:17:43    | C++        |              19 |            13 |
| sumoru          | 924813    |     18 | 03.24.2019 06:30:07    | C++        |              17 |             7 |
| yarrr           | 923333    |     19 | 03.24.2019 09:11:44    | C++        |              14 |             6 |
| tashikani       | 922789    |     20 | 03.24.2019 10:53:13    | C++        |              10 |             5 |
| Daiver19        | 916611    |     21 | 03.23.2019 17:13:05    | C++        |              15 |             6 |
| kurenai3110     | 908200    |     22 | 03.24.2019 11:37:07    | C++        |               4 |             4 |
| Stefan70        | 905062    |     23 | 03.23.2019 19:04:30    | Java       |               4 |             4 |
| __math          | 904142    |     24 | 03.21.2019 05:26:46    | C++        |               0 |             2 |
| H_a_s_h         | 899536    |     25 | 03.22.2019 15:00:46    | C++        |               3 |             3 |
| _sash           | 886232    |     26 | 03.23.2019 07:54:35    | C++        |               9 |             5 |
| tsukasa_diary   | 883545    |     27 | 03.24.2019 11:43:33    | C++        |              11 |             7 |
| kwrig           | 876106    |     28 | 03.24.2019 10:12:30    | Java       |               7 |             7 |
| arukuka         | 871969    |     29 | 03.24.2019 09:06:43    | C++        |               9 |             6 |
| okazaki         | 869188    |     30 | 03.23.2019 00:10:39    | Java       |               0 |             8 |
| my316g          | 857641    |     31 | 03.19.2019 17:55:23    | C++        |               4 |             4 |
| nel215          | 857632    |     32 | 03.24.2019 02:10:10    | C++        |               2 |             7 |
| roto37          | 852689    |     33 | 03.22.2019 13:55:34    | C++        |               6 |             4 |
| dtaka           | 849252    |     34 | 03.23.2019 11:45:00    | Java       |               1 |             1 |
| kozima          | 834092    |     35 | 03.20.2019 12:13:18    | C++        |               5 |             4 |
| AmAtUrECoDeR    | 832101    |     36 | 03.24.2019 00:43:48    | Java       |              19 |             5 |
| yuya178         | 830563    |     37 | 03.21.2019 02:29:44    | Java       |              18 |            10 |
| olphe           | 828118    |     38 | 03.24.2019 10:37:33    | C++        |              55 |            15 |
| fine_           | 820494    |     39 | 03.24.2019 11:23:36    | C++        |               4 |             2 |
| iehn            | 816489    |     40 | 03.17.2019 08:44:16    | C++        |              15 |             3 |
| HrKdr           | 814030    |     41 | 03.22.2019 12:48:18    | C++        |               5 |             6 |
| windhunterSB    | 805071    |     42 | 03.21.2019 19:12:59    | C++        |               5 |             5 |
| Aruru           | 799941    |     43 | 03.22.2019 06:34:38    | C++        |               4 |             4 |
| bduvenhage      | 797927    |     44 | 03.23.2019 02:38:59    | C++        |               8 |             6 |
| kishore_g84     | 784056    |     45 | 03.21.2019 02:01:50    | Java       |               3 |             6 |
| KShiv           | 773305    |     46 | 03.24.2019 06:16:09    | Java       |               8 |             5 |
| kruntuid        | 770245    |     47 | 03.24.2019 12:02:41    | C++        |               4 |             7 |
| vi002           | 740869    |     48 | 03.19.2019 04:06:21    | C++        |               4 |             4 |
| sullyper        | 691896    |     49 | 03.22.2019 02:53:32    | C++        |               3 |             3 |
| ValGrowth       | 691805    |     50 | 03.24.2019 11:59:01    | C++        |               3 |             8 |
| ty70            | 677441    |     51 | 03.23.2019 15:56:30    | C++        |               6 |             4 |
| ArtDitel        | 669915    |     52 | 03.19.2019 08:03:19    | C++        |               3 |             1 |
| fetetriste      | 656526    |     53 | 03.19.2019 16:19:27    | Java       |               1 |             1 |
| WhiteParadise   | 648706    |     54 | 03.23.2019 19:17:44    | Java       |               3 |             3 |
| abdullahkool768 | 629924    |     55 | 03.19.2019 01:26:43    | C++        |               3 |             2 |
| seess_yshysj    | 610894    |     56 | 03.19.2019 08:05:24    | C++        |              14 |             8 |
| mmammel         | 588253    |     57 | 03.19.2019 15:47:41    | C++        |               5 |             4 |
| mcw1142         | 563845    |     58 | 03.21.2019 21:19:26    | Java       |               7 |             6 |
| wvsever         | 491240    |     59 | 03.21.2019 12:45:25    | C++        |               7 |             3 |
| TomA-_-         | 473105    |     60 | 03.24.2019 06:03:08    | C#         |              25 |            11 |
| nii_nii         | 472232    |     61 | 03.19.2019 09:38:25    | C++        |              11 |             3 |
| dhalachian      | 456626    |     62 | 03.23.2019 17:48:33    | C#         |               3 |             3 |
| neetsdkasu      | 452842    |     63 | 03.24.2019 11:55:21    | VB         |               3 |             3 |
| daisyo          | 401988    |     64 | 03.24.2019 04:46:21    | C++        |               1 |             1 |
| nagyt           | 388181    |     65 | 03.20.2019 18:01:33    | C++        |               1 |             1 |
| wadissimo       | 341927    |     66 | 03.17.2019 16:11:18    | Java       |               1 |             1 |
| SavinovAlex     | 264297    |     67 | 03.23.2019 13:26:11    | C++        |               5 |             2 |
| usto            | 244315    |     68 | 03.23.2019 14:31:22    | Python     |              18 |             5 |
| Arterm          | 223586    |     69 | 03.23.2019 19:33:20    | C++        |               0 |             1 |
| adist98         | 204534    |     70 | 03.21.2019 05:22:34    | C++        |              34 |            14 |
| seibu_md_4562   | 145073    |     71 | 03.21.2019 19:54:44    | C#         |               4 |             1 |
| fmhr            |  51378.9  |     72 | 03.23.2019 02:02:41    | C++        |               1 |             3 |
| zemen           |   3725.82 |     73 | 03.23.2019 19:16:19    | C++        |               1 |             1 |
| DebNatkh        |   2461.07 |     74 | 03.17.2019 17:57:30    | C++        |               0 |             1 |
| the_lizard      |    900.58 |     75 | 03.17.2019 08:57:38    | C++        |               1 |             1 |
| Hari_Om_Pandey  |      0    |     76 | 03.21.2019 15:57:07    | Python     |               2 |             2 |
| id              |      0    |     76 | 03.18.2019 14:27:32    | C#         |               1 |             1 |
| tuff            |      0    |     76 | 03.24.2019 06:08:10    | C++        |               1 |             2 |
| *phocom         |           |        | 03.24.2019 12:12:06    | C++        |               1 |             1 |
| r2d1            |           |        |                        |            |               1 |             0 |
| tobari          |           |        |                        |            |               1 |             0 |

</details>

<details>
<summary>画像: 焼きなまし, 集合を単調にする</summary>

[a095cda41bab002963479da57c35f3720141e931](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/a095cda41bab002963479da57c35f3720141e931)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/a095cda41bab002963479da57c35f3720141e931.10.png)

</details>

<details>
<summary>画像: 焼きなまし, 前処理を P / M でやる</summary>

[473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376](https://github.com/kmyk/topcoder-marathon-match-109-road-network/commit/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376)

![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.1.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.2.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.3.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.4.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.5.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.6.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.7.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.8.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.9.png)
![](https://github.com/kmyk/topcoder-marathon-match-109-road-network/blob/627f0870ee4ac22b2954ce1ac4c4357dfb63c66a/images/473d3a3e57e65e77d6dbda7cf1fb52b1b6b7b376.10.png)

</details>
