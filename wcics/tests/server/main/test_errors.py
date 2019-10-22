def test_404(client):
  assert client.get("/sdgayidsa").status_code == 404