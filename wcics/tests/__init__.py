import wcics.server.routes

from wcics.utils.time import get_time
from wcics.database.models import Organizations
from wcics.database.utils import db_commit

## Perform any init that is assumed to exist

# Create main organization
Organizations.add(oid = 'main', name = 'Main Organization', desc = '', create_time = get_time())
db_commit()