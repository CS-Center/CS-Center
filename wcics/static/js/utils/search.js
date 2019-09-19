function fuzzy(search, string) {
  if (search === "") return 1;
  var index = 0;
  for (var x = 0; x < string.length; x++) {
    if (string[x] == search[index]) {
      index++;
      if (index == search.length) {
        return 1;
      }
    }
  }
  return 0;
}

function LCS(search, string) {
  N = string.length;
  M = search.length;
  var table = [];
  for (i = 0; i <= N; i++) {
    table.push([]);
    for (j = 0; j <= M; j++) {
      if (i === 0 || j === 0) {
        table[i].push(0);
      } else if (string[i - 1] === search[j - 1]) {
        table[i].push(table[i - 1][j - 1] + 1);
      } else { 
        table[i].push(Math.max(table[i - 1][j], table[i][j - 1]));
      }
    }
  }
  return table[N][M] >= Math.max(M - 1, 0);
}