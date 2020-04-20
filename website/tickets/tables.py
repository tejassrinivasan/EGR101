import django_tables2 as tables
from .models import Ticket
import itertools

class RequestsTable(tables.Table):
	class Meta:
		model = Ticket
		template_name = "django_tables2/bootstrap.html"
		fields = ("id", "room", "request", "pub_date", "status",)

"""counter = tables.Column(empty_values=(), orderable=False)

	def render_counter(self):
		self.row_counter = getattr(self, 'row_counter',
			itertools.count(self.page.start_index()))
		return next(self.row_counter)
"""