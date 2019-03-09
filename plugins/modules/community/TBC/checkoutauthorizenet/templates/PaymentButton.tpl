 <form action="{g->url}" method="post">
  {g->hiddenFormVars}
  <input type="hidden" name="{g->formVar var="controller"}" value="checkoutauthorizenet:Details" />
  <input type="submit" name="{g->formVar var="form[action][email]"}" value="{g->text text="Checkout"}" />
</form>